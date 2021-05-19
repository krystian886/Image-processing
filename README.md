# Image-processing

Written in VS 2019 as C++/CLR Console App (.NET framework)<br>
<b>!! Not tested anywhere else than VS 2019 !!</b><br>
<b>!! Not optimised for big images !!</b>

# Algorithms:
  - [Normalization](#normalization)
  - [Standard deviation filtering](#standard-deviation-filtering)
  - [Opening](#opening) (morphology)
  - [Connected component labeling](#connected-component-labeling) ( 8 directions )

# Normalization
## Description
wiki description: https://en.wikipedia.org/wiki/Normalization_(image_processing) <br>
This programme uses input image's pixels as values ranged on a X axis,<br>
while the output is located on a Y axix. Values are being transfered with the help of<br>
Y = a*X + b functions. Those functions are being created just as shown below, using input points, <br>
provided by the programme' user.<br><br>
<b>Points (0,0) and (255,255) are hard-coded.</b>
<br>
![graph](images/wykres.png)

## Examples
### RGB
![boat](images/boat.png)
![boatNew](images/normalizacjaRGB_new.png)
### Monochromatic
![skate](images/skate.png)
![skateNew](images/normalizacjaMono_new.png)

# Standard-deviation-filtering
## Description
wiki description: https://en.wikipedia.org/wiki/Gaussian_filter <br>
This programme is also very simple. It's purpose is to scan through the input image' pixels<br>
using a mask (user chooses its size). When the mask works on a specific pixel, it gathers <br>
the data from its neighbours, to calculate their standard-deviation, then quick normalization <br>
happens (to spread the data evenly across the (0,255) range) and that is all. This value is <br>
then saved to the output image. <br>

<b>Because the mask needs all of its values to work, when choosing a bigger mask, the output image <br>
gains a black frame around itself.</b>

## Examples
### RGB
![onion](images/onion.png)
![onionNew](images/filtracjaRGB_new.png)
### Monochromatic
![circuit](images/circuit.png)
![circuitNew](images/filtracjaMono_new.png)

# Opening
## Description
wiki description: https://en.wikipedia.org/wiki/Opening_(morphology) <br>
https://en.wikipedia.org/wiki/Dilation_(morphology) <br>
https://en.wikipedia.org/wiki/Erosion_(morphology) <br>
Opening is an operation that looks like this: <br>
    opening = dilation ( ( erosion, SE ), SE ) <br>
and SE - is a structural element, which picks the pixels around the <br>
computed one to find its new value. <br>
In my case SE is a disk created by the equation: <br>
          (x-a)^2+(y-b)^2=r2 <br>
radius: 6 example: <br><br>
![mask](images/maska.png) <br>
<b>This time there is no black frame around the new image, because <br>
I've decided to let the mask create new pixels even when the values are not complete. <br>

## Examples
### Logic
![circles](images/circles.png)
![circlesNew](images/otwarcieLog_new.png)
### Monochromatic
![circuit](images/circuit.png)
![circuitNew](images/otwarcieMono_new.png)

# Connected-component-labeling
## Description
wiki description: https://en.wikipedia.org/wiki/Connected-component_labeling <br>
The last algorithm searches for items and labels them. It then allows to for example <br>
count them or find if they are connected. There are two versions of the algorithm: <br>
  - 4 connections check: up, down, left, right, <br>
  - 8 connections check: above + diagonals. <br>
  
 My version is the 8 connections check. <br>
 This algorithm is recursive, and so: it is fast BUT it only works with small images. <br>

## Examples
### Logic
![circles](images/coins.png)
<br>
Result: 10 - printed out in a console And saved as .txt file.
