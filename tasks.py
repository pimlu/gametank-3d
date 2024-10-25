from invoke import task

from os import path
import io
import re

root_dir = path.dirname(__file__)


@task
def clean(c):
    with c.cd(root_dir):
        c.run("rm -rf build/")


@task
def ensure_container_running(c):
    result = c.run(r'podman ps --filter "name=gametank" --filter "status=running" --format "{{.Names}}"')
    
    if not result.stdout.strip():
        print('no container found, starting...')
        with c.cd(root_dir):
            c.run('podman run -d --name gametank -v $(pwd):/workspace:z dwbrite/rust-mos:gametank-edition sleep infinity')


def list_glob(c, dir, pattern):
    result = c.run(f"find {dir}", hide=True)
    output = result.stdout.split('\n')
    return [f for f in output if re.match(pattern, f)]

def gt_run(c, cmd):
    return c.run(f"podman exec -t -w /workspace gametank {cmd}")

def build_asm_dir(c, dir):
    objs = []
    with c.cd(root_dir):
        asm_files = list_glob(c, f"src/{dir}", r".*\.asm$")
        for src in asm_files:
            dst = path.join('build', src)
            dst = re.sub(r'\.asm$', '.o', dst)

            c.run(f"mkdir -p {path.dirname(dst)}")
            # can't use the magic -mcpu cause it hangs forever...
            gt_run(c, f"llvm-mc --filetype=obj -triple=mos '{src}' -o '{dst}'")

            objs.append(dst)
    return objs


def build_cpp_dir(c, dir):
    objs = []
    with c.cd(root_dir):
        asm_files = list_glob(c, f"src/{dir}", r".*\.cpp$")
        for src in asm_files:
            dst = path.join('build', src)
            dst = re.sub(r'\.cpp$', '.o', dst)

            c.run(f"mkdir -p {path.dirname(dst)}")
            # magic -mcpu came from llvm-mos pull #192
            gt_run(c, f"clang --std=c++17 -Os -fno-stack-protector -c -Xclang -triple=mos -mcpu=mosw65c02 -isystem /usr/local/mos-platform/common/include -Isrc '{src}' -o '{dst}'")

            objs.append(dst)
    return objs

def build_dir(c, dir):
    asm_o = build_asm_dir(c, dir)
    cpp_o = build_cpp_dir(c, dir)
    return asm_o + cpp_o

@task
def build(c):
    ensure_container_running(c)

    drawbox_o = build_dir(c, "games/drawbox")
    system_o = build_dir(c, "system")

    objs = drawbox_o + system_o

    with c.cd(root_dir):
        obj_args = ' '.join(f"'{o}'" for o in objs)
        ### original command used to figure out the linker command:
        # gt_run(c, f'clang --verbose -Xclang -triple=mos -T link.ld -L /usr/local/mos-platform/common/lib -o build/drawbox {obj_args}')
        ### original linker command:
        # gt_run(c, '"/usr/local/bin/ld.lld" --gc-sections --sort-section=alignment build/src/games/drawbox/main.o build/src/system/interrupts.o build/src/system/bcr.o build/src/system/via.o build/src/system/boot.o build/src/system/scr.o build/src/system/types.o -plugin-opt=-function-sections=1 -plugin-opt=-data-sections=1 -mllvm -force-precise-rotation-cost -mllvm -jump-inst-cost=6 -mllvm -force-loop-cold-block -mllvm -phi-node-folding-threshold=0 -mllvm -speculate-blocks=0 -mllvm -align-large-globals=false -mllvm -disable-spill-hoist -mllvm -lsr-complexity-limit=10000000 -L/usr/local/lib/clang/19/lib/mos-unknown-unknown -T link.ld -L/usr/local/mos-platform/common/lib -l:crt0.o -lcrt0 -lcrt -lc -o build/drawbox')
        ### modified linker command: (took out some libraries)
        gt_run(c, f'ld.lld --sort-section=alignment {obj_args} -plugin-opt=-function-sections=1 -plugin-opt=-data-sections=1 -mllvm -force-precise-rotation-cost -mllvm -jump-inst-cost=6 -mllvm -force-loop-cold-block -mllvm -phi-node-folding-threshold=0 -mllvm -speculate-blocks=0 -mllvm -align-large-globals=false -mllvm -disable-spill-hoist -mllvm -lsr-complexity-limit=10000000 -L/usr/local/lib/clang/19/lib/mos-unknown-unknown -T link.ld -L/usr/local/mos-platform/common/lib -l:crt0.o -lcrt0  -o build/drawbox')
        gt_run(c, f'llvm-objcopy -O binary build/drawbox build/drawbox.gtr')
    

@task
def build_test(c):
    with c.cd(root_dir):
        c.run("mkdir -p build/tests")
        c.run("clang++ --std=c++17 -lstdc++ -fsanitize=undefined -Isrc src/graphics/bresenham.cpp src/tests/bresenham_test.cpp -o build/tests/bresenham_test")


@task
def objdump(c):
    gt_run(c, f'llvm-objdump -D --triple=mos build/drawbox')
