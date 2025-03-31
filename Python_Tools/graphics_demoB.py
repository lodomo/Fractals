from ctypes import *
from Gkit import *

  
swidth = 500
sheight = 400

G_init_graphics (swidth,sheight) # must do this BEFORE any other G_ calls

# clear screen in a given color
G_rgb (0.3, 0.3, 0.3) # dark grey
G_clear ()

# draw a point
G_rgb (1,0,0) # red
G_point (200,380)

# draw a line
G_rgb (0,1,0) # green
G_line (0,0,swidth-1,sheight-1)

# aligned rectangles
G_rgb (0,0,1) # blue
lowleftx = 200
lowlefty = 50
width = 10
height = 30
G_rectangle (lowleftx, lowlefty, width, height)
lowleftx = 250
G_fill_rectangle (lowleftx, lowlefty, width, height)

# triangles
G_rgb (1,1,0) # yellow
G_triangle (10,300, 40,300,  60,250)
G_fill_triangle (10,100,  40,100,  60,150)

# circles
G_rgb (1,0.5,0)  # orange
G_circle (100,300, 75)
G_fill_circle (370,200, 50)

# polygons

# construct a polygon :
x = [100,100,300,300,200]
y = [100,300,300,100,175]
G_rgb (0,0,0)  # black
G_polygon (x,y)

G_rgb (1,1,1) # white
G_draw_string ("Hit any key to continue", 25, 55) ;
k1 = G_wait_key() # pause to see picture so far
print("key = ", k1)

# construct another polygon :
a = [400,450,375,225]
b = [380,250,225,350]
G_rgb (0.4, 0.2, 0.1) # brown
G_fill_polygon (a,b)

#========================================================


p=[0,0] # need 2 slots, values immaterial...will be overwritten
q=[0,0] # need 2 slots, values immaterial...will be overwritten

G_rgb (1,1,1) # white
G_draw_string ("Click the mouse", 25, 40) ;

G_wait_click(p) # wait for a mouse click in the image
G_fill_circle(p[0],p[1],2) ;

G_rgb (1,1,1) # white
G_draw_string ("Click the mouse again", 25, 25) ;

G_wait_click(q) # wait for another mouse click in the image
G_fill_circle(q[0],q[1],2) ;

G_rgb (0,1, 0.5) # bluish green
G_line (p[0],p[1], q[0],q[1]) 

G_rgb (1,1,1) # white
G_draw_string ("Any key to continue", 25, 10) ;

k2 = G_wait_key() # pause again to see picture so far
print("key = ", k2)

G_save_image_to_file("demo.xwd")

fname = "demo.bmp"
G_save_to_bmp_file(fname)

print("Graphics window closing")
G_close()


answer = input("Would you like the time of day? (yes, else no) ")

if answer == "yes" :
  hms = [0,0,0]  # need 3 slots, values immaterial...will be overwritten
  get_time(hms)
  print(hms[0], " hours and ", hms[1], " minutes and ", hms[2], " seconds") ;


