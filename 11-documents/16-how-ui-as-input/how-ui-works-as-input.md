# How UI module works as Input?




## Code Changes

1. key datatype is `DIO_STRUCT`

![1](image.png)

2. Make an array and assign each input an unique ID

`UI_INPUT_STRU ui_input;`

![2](image-1.png)

![3](image-2.png)


3. In the hardware pin interrupt callback, update the array.

This example shows a 3-position radio group, which is assigned with 4 IDs: it has 3 physical pins, but uses a logical 4th ID as the data to UI upper level. 

![4](image-4.png)

A macro function is used to record pin status to dio_stru array.

`#define ui_record_pin(ID, level) ui_dio_set_value(&(ui_input.pin[ID]), level);`

> Note here, the `value_last` is not used. The reason is that, this module actively read the pin status, instead of passively. Then, it could be deleted.

![5](image-5.png)


# END
