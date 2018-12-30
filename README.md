# PPM_Photoshop
A PPM photo editor: blur, contrast, zoom(in/out), occlude a region, swap color channels

Command Line Arguments:
		
	./project in_file(.ppm) out_file(.ppm) operation-name (paramaters)
	
Funtions and Parameters:
	
	Swap 
	 - none
	 - swap multiple times to get different color channels
	
	Grayscale 
	 - none
		
	Contrast 
	 - any float value
	 - absolute value of contrast value results in higher contrast
		
	Zoom_In/Zoom_Out 
	 - none	
	 - quadruples or quarters image size
		
	Occlude 
 	 - 4 integers, the coordinates of oposing corners of a rectangle to black-out
	 - (start_col, start_row) (end_col, end_row)
	 	
	Blur 
	 - float value > 0
	 - larger sigma for more intense blur
	
