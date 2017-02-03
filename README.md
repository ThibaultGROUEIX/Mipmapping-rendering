### Viewing the consequence of mipmapping on rendered view

to run this code:
``` sh
$ git clone https://github.com/ThibaultGROUEIX/Mipmapping-rendering.git
$ cd Mipmapping-rendering/build
$ make
$ ./main
```

### Usage

Change the view point with the mouse. (the man is originally between the viewpoint and the light so he will appear black as there is no reflexion. To view the man, we have to change the viewpoint a tiny bit).

Strike "m" key to change the level of mipmapping of one frame of the G-buffer (can be position, normal, or albedo)
Observe the change in the rendered view (you can observe a nice specular spot)

### What happens ?

The rendering is done by rasterization. Each pixel value is computed by computing the BRDF function. This function depends on the normal, position and albedo. If they are stored in a down-sampled way the render view is less good (bad contours especcially) but the render time is better. This proggram can help you understand what is the best level of mipmapping for albedo, normal and position respectively for you to reach the quality of rendering you are aiming for.

[![Analytics](https://ga-beacon.appspot.com/UA-91308638-2/github.com/ThibaultGROUEIX/Mipmapping-rendering/README?pixel)](https://github.com/ThibaultGROUEIX/Mipmapping-rendering/)
