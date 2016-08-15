#include <md/md.hpp>

//256-width = colorburst * 15 / 10
//320-width = colorburst * 15 /  8

namespace MegaDrive {

VDP vdp;
#include "io.cpp"
#include "dma.cpp"
#include "render.cpp"
#include "background.cpp"
#include "sprite.cpp"

auto VDP::Enter() -> void {
  while(true) scheduler.synchronize(), vdp.main();
}

auto VDP::main() -> void {
  scanline();
  if(state.y < 240) {
    if(state.y == 0) {
      cpu.lower(CPU::Interrupt::VerticalBlank);
    }
    cpu.lower(CPU::Interrupt::HorizontalBlank);
    for(uint x : range(320)) {
      run();
      step(1);
    }
    if(io.horizontalBlankInterruptEnable) {
      cpu.raise(CPU::Interrupt::HorizontalBlank);
    }
    step(22);
  } else {
    if(state.y == 240) {
      if(io.verticalBlankInterruptEnable) {
        cpu.raise(CPU::Interrupt::VerticalBlank);
      }
    }
    step(342);
  }
}

auto VDP::step(uint clocks) -> void {
  while(clocks--) {
    dmaRun();
    Thread::step(1);
    synchronize(cpu);
  }
}

auto VDP::refresh() -> void {
  Emulator::video.refresh(buffer, 1280 * sizeof(uint32), 1280, 480);
}

auto VDP::power() -> void {
  planeA.power();
  window.power();
  planeB.power();
  sprite.power();
}

auto VDP::reset() -> void {
  create(VDP::Enter, system.colorburst() * 15.0 / 10.0);

  memory::fill(&io, sizeof(IO));

  planeA.reset();
  window.reset();
  planeB.reset();
  sprite.reset();
}

}
