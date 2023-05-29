#pragma once

#include <hal/vmm.h>
#include <handover/builder.h>
#include <karm-base/rc.h>
#include <karm-base/res.h>

namespace Loader::Fw {

Res<Strong<Hal::Vmm>> createVmm();

Res<> finalizeHandover(Handover::Builder &builder);

void enterKernel(usize entry, Handover::Payload &payload, usize stack, Hal::Vmm &vmm);

Hal::PmmRange imageRange();

} // namespace Loader::Fw
