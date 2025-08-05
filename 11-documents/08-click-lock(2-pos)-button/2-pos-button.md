# 2-Pos Button / Switch

The default button is 'click-type', this 'click-lock' type remember the button on-off state through your software (the is_enable data in UI local data), It shows the switch/lock state by changing the button color.

![effect](image-1.png)

## Code Change

1. you need to maintain the switch state in ui:

<img src="image.png" border=1>

2. when the button is clicked (as a normal button click), you track the state change. use a switch operator when you have more buttons, and in each 'case', you call invalidate() to trigger the redraw event.

<img src="image-2.png" border=1>


3. in the redraw event, draw the button according to the state


<img src="image-4.png" border=1>
<img src="image-3.png" border=1>



# The end



