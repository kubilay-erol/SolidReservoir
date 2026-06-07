# SolidReservoir

A lightweight embedded-friendly thermal state tracker for microcontrollers without an FPU.

## Overview

Models a solid thermal reservoir with known material properties. Given heat absorbed, it calculates the resulting temperature change using `ΔT = Q/(m·c)`. Useful when direct temperature sensing is impractical — mount a sensor on a nearby accessible component, measure heat flow, and infer temperature of the target device.

## Fixed-Point Arithmetic

All physical quantities use the `fixed` class, which stores values as scaled `int32_t` internally. This avoids floating point entirely, making it suitable for bare-metal targets like STM32 without FPU.

## Units

| Parameter | Unit |
|---|---|
| `temp` | K (consistent use is caller's responsibility) |
| `mass` | g |
| `c` | J/g·K |
| `heat` | J |

## Usage

```cpp
#include "fixed.h"
#include "SolidReservoir.h"

// allocate memory for the object
alignas(SolidReservoir) uint8_t buf[sizeof(SolidReservoir)];

SolidReservoir* engine = SolidReservoir::create_reservoir(
    reinterpret_cast<SolidReservoir*>(buf),
    "engine block",
    fixed(20),   // initial temp
    0,           // 0 = finite, 1 = infinite reservoir
    fixed(10),   // mass in g
    fixed(500)   // specific heat capacity J/g·K
);

// when heat is absorbed
engine->heat_transfer(fixed(1000)); // 1000J absorbed

// read temperature
fixed t = engine->temp();
```

## Notes

- Heat loss to the environment is the caller's responsibility — `heat_transfer` only models heat absorbed, not dissipated
- Infinite reservoirs (`size = 1`) do not change temperature regardless of heat input
- `fixed` has a precision of 3 decimal places (scaled by 1000 internally)
