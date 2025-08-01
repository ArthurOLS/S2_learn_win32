# Add a Label

Key takeway: 
1. How it works? in the ui.c/h module, make a function and call `CreateWindow` to create the label. Main.cpp call this function in WM_CREATE event once. 
2. **Everything** on the screen is a '**Windows**'
## 1. Outcome

![0](image-5.png)

## 2. Code changes

### 1 In ui module

to use `HWND` type.

![1](image-2.png)

![2](image-3.png)

Declaration in .h file

![3](image-4.png)

### 2 In the main

![1](image.png)

![2](image-1.png)




















