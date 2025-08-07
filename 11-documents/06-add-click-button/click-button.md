# Add click-type button

A click-type button is the default button type, it generate a 'WM_COMMAND' event when you lift the click.

This demo output a message when a button is click, the output window is in Debug/View/Output menu.

![1](image.png)

## Code Change
1. define the button IDs for each button (used for callback)

![2](image-1.png)


2. called by SandboxUI.cpp/WM_CREATE event, the button window control is created

    `hLabel1 = CreateWindowA("STATIC", "Floor: 0", WS_CHILD | WS_VISIBLE,
                                20, 20, 120, 20, hwnd, NULL, NULL, NULL);`

    - "STATIC": Create a label (non-editable text).
    - "Floor: 0": The initial text to display.
    - WS_CHILD | WS_VISIBLE: Make it a child control of the main window, and make it visible.
    - 20, 20, 120, 20: Position and size (X, Y, Width, Height)
    - hwnd: Parent window.
    - NULL values for menu/ID for now (could be upgraded).
 
![3](image-2.png)

1. create a button callback function
   
![5](image-4.png)

4. Call the callback in WM_COMMAND event. 

![4](image-3.png)

## Difference between CreateWindowA() and CreateWindowW()?

```
    This comes down to character encoding:
    CreateWindowA()	ANSI	Uses 8-bit char strings
    CreateWindowW()	Wide	Uses 16-bit wchar_t (Unicode)
    CreateWindow()	Macro	Resolves to A or W depending on compiler settings
    Conclusion: simple, Always prefer Unicode when building Windows apps
```
---- The end
