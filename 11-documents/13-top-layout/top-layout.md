# How I set top-level layout

![1](image.png) 

## Draft Design

Use drawio to finish the primative layout design first.

1. Divide the window into several smaller blocks, I prefer several columns.
2. Decide placing what widgets into each block
3. get each block's (x,y,w,h) --- from the 'arrange' sidebar

![1](ui_layout-Page-1.drawio.png)

![3](image-1.png)

## Code Change


1. set windows size, this is the outer size, a little larger than client area size.

 <img src="image-2.png" border=1 width=400>


2. set the block basic size, here I use 'columns' as blocks

![alt text](image-3.png)

3. set each block's (x,y,w,h)

![3](image-4.png)

4. in ui.cpp, draw all the lables with block (x,y,w,h) settings

![4](image-5.png)


# END














