**Ground Truth Selector v1.3:**
- New layout
- Add left toolbar (with new widget to setup pen size and pen color)
- Add menu (just open and close, todo: more actions)
- Adding ROI to labelList, showing label, id, size and enabled checkbox.
- Add select ROI and delete ROI action
- Fix moving ROI with handlers (todo: when created, saving save wrong position)
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.3_1.png)
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.3_2.png)
**Ground Truth Selector v1.2:**
- Saving changes made in current view to json files - fix bugs
- Add snap to grid while moving ROI
- Add file information (numbers of ROI, number of GT and SHADOW pixels)
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.2_1.png)
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.2_2.png)
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.2_3.png)
**Ground Truth Selector v1.1:**
- Saving changes made in current view to json files
![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.1_1.png)

**Ground Truth Selector v1.0:**

- Load images to application:

![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.0_2.png)

- Select ROI and save it to json file

![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.0_3.png)

- Manually select ground truth pixel and save it to json file
- Automatically apply image processing function to background subtraction configurable via config

![alt text](https://github.com/SanczoPL/gt/blob/master/doc/v1.0_4.png)

*Config file:*
```python
{
	
	"Colors":
	{
		"Black": 0,
		"White": 255,
		"Shadow":50,
		"OutOfScope":85,
		"Unknown":170
	},
        "General":
        {
            "LogLevel" : 0
        },
        "Dataset":
        {
            "InputFolder": "",
            "OutputFolder": "",
            "OutputFolderGT": "",
            "OutputFolderDiff": "",
            "OutputFolderJSON": ""
        },
	"PostProcessing":
	{
		"Name": "MorphologyOperation",
		"MorphOperator": 1,
		"MorphSize": 1,
		"MorphElement": 0
	},
	"PreProcessing":
	{
		"Name": "MedianBlur",
		"KernelSize": 5
	},
	"BackgroundSubtractor":
	{
		"Name:" : "MOG2",
		"LearningRate":0.9,
		"History":50,
		"VarThreshold":109,
		"DetectShadow":true
	}
}
```
