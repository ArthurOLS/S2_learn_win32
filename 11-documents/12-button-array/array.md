# 



## How to change buttons?
Below creates 6 buttons labeled "Hall 1" to "Car 3".
```c
const char* labels[6] = { "Hall 1", "Hall 2", "Hall 3", "Car 1", "Car 2", "Car 3" };
for (int i = 0; i < 6; ++i) {
    hButtons[i] = CreateWindowA("BUTTON", labels[i], WS_VISIBLE | WS_CHILD,
        20 + (i % 3) * 70, 100 + (i / 3) * 40, 60, 30,
        hwnd, (HMENU)(100 + i), NULL, NULL);
}
```
- 20 + (i % 3) * 70: Space buttons horizontally, 3 per row.
- (i / 3) * 40: Adds a second row after 3 buttons.
- 60x30: Width and height.
- (HMENU)(100 + i): Each button gets a unique control ID starting from 100. use these in WM_COMMAND to know which button was clicked.

