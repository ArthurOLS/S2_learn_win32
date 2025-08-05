# Add click-type button

A click-type button is the default button type, it generate a 'WM_COMMAND' event when you lift the click.

This demo output a message when a button is click, the output window is in Debug/View/Output menu.

![1](image.png)

## Code Change
1. define the button IDs for each button (used for callback)

![2](image-1.png)


2. called by SandboxUI.cpp/WM_CREATE event, the button window control is created

![3](image-2.png)

3. create a button callback function
   
![5](image-4.png)

4. Call the callback in WM_COMMAND event. 

![4](image-3.png)



---- The end
