# Continuous-press type button
This type of button is used: <br>
To detect both button press and release events (like bool up = 1 on press and up = 0 on release),

> I initially used this 'SetWindowSubclass' solutioin, which is said to be safer and compatiable across all windows OSes. 

![1](image.png)


## Code Change (all code is in ui.cpp)

1. Include the library

![2](image-1.png)

2. Create buttons and keep their HWNDs

You need to track the state of the press buttons, win31 does NOT do this for you.

![3](image-2.png)

![4](image-3.png)

3. Create the callback, this can sit anywhere in your project, to make it simple, just put it in ui.cpp


![4](image-4.png)

or using the button id (prefered solution) to identify between buttons.

![5](image-5.png)

# end



