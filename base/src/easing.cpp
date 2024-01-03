/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#include <base/easing.h>

namespace base
{
	static const EaseFn s_easeFunc[] =
	{
		easeLinear,
		easeStep,
		easeSmoothStep,
		easeInQuad,
		easeOutQuad,
		easeInOutQuad,
		easeOutInQuad,
		easeInCubic,
		easeOutCubic,
		easeInOutCubic,
		easeOutInCubic,
		easeInQuart,
		easeOutQuart,
		easeInOutQuart,
		easeOutInQuart,
		easeInQuint,
		easeOutQuint,
		easeInOutQuint,
		easeOutInQuint,
		easeInSine,
		easeOutSine,
		easeInOutSine,
		easeOutInSine,
		easeInExpo,
		easeOutExpo,
		easeInOutExpo,
		easeOutInExpo,
		easeInCirc,
		easeOutCirc,
		easeInOutCirc,
		easeOutInCirc,
		easeInElastic,
		easeOutElastic,
		easeInOutElastic,
		easeOutInElastic,
		easeInBack,
		easeOutBack,
		easeInOutBack,
		easeOutInBack,
		easeInBounce,
		easeOutBounce,
		easeInOutBounce,
		easeOutInBounce,
	};
	BASE_STATIC_ASSERT(BASE_COUNTOF(s_easeFunc) == Easing::Count);

	EaseFn getEaseFunc(Easing::Enum _enum)
	{
		return s_easeFunc[_enum];
	}

} // namespace base
