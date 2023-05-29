#pragma once

#include "types.h"

namespace Hj {

Res<> _log(char const *msg, usize len);

Res<> _create(Cap dest, Cap *cap, Props const *props);

Res<> _label(Cap cap, char const *label, usize len);

Res<> _drop(Cap cap);

Res<> _dup(Cap node, Cap *dst, Cap src);

Res<> _start(Cap cap, usize ip, usize sp, Args const *args);

Res<> _map(Cap cap, usize *virt, Cap vmo, usize off, usize *len, MapFlags flags = MapFlags::NONE);

Res<> _unmap(Cap cap, usize virt, usize len);

Res<> _in(Cap cap, IoLen len, usize port, Arg *val);

Res<> _out(Cap cap, IoLen len, usize port, Arg val);

Res<> _send(Cap cap, Msg const *msg, Cap from);

Res<> _recv(Cap cap, Msg *msg, Cap fo);

Res<> _close(Cap cap);

Res<> _signal(Cap cap, Flags<Sigs> set, Flags<Sigs> unset);

Res<> _watch(Cap cap, Cap target, Flags<Sigs> set, Flags<Sigs> unset);

Res<> _listen(Cap cap, Event *ev, usize evLen, TimeStamp deadline);

} // namespace Hj
