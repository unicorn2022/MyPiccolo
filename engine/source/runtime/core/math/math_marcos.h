#pragma once

#define MYPICCOLO_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MYPICCOLO_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MYPICCOLO_PIN(a, min_value, max_value) MYPICCOLO_MIN(max_value, MYPICCOLO_MAX(a, min_value))

#define MYPICCOLO_VALID_INDEX(idx, range) (((idx) >= 0) && ((idx) < (range)))
#define MYPICCOLO_PIN_INDEX(idx, range) MYPICCOLO_PIN(idx, 0, (range)-1)

#define MYPICCOLO_SIGN(x) ((((x) > 0.0f) ? 1.0f : 0.0f) + (((x) < 0.0f) ? -1.0f : 0.0f))
