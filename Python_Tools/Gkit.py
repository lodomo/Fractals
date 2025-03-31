import ctypes
from ctypes import *

FPTlib = ctypes.CDLL("./FPT.so")



def jcopy (a,b, n):
  i = 0
  while i < n :
    a[i] = b[i]
    i=i+1



def G_init_graphics(w,h):
    w = FPTlib.G_init_graphics(ctypes.c_double(w), ctypes.c_double(h))

def G_clear():
    w = FPTlib.Clear_Buffer_X()

def G_rgb(r,g,b):
    w = FPTlib.Set_Color_Rgb_DX(ctypes.c_double(r),ctypes.c_double(g),ctypes.c_double(b))
    
def G_point(x,y):
    w = FPTlib.Safe_Point_X(ctypes.c_double(x),ctypes.c_double(y))
    
def G_line(xs,ys,xe,ye):
    w = FPTlib.Line_X(ctypes.c_double(xs),ctypes.c_double(ys),ctypes.c_double(xe),ctypes.c_double(ye))    

def G_rectangle(xl,yl,w,h):
    w = FPTlib.Rectangle_X(ctypes.c_double(xl),ctypes.c_double(yl),ctypes.c_double(w),ctypes.c_double(h))

def G_fill_rectangle(xl,yl,w,h):
    w = FPTlib.Fill_Rectangle_X(ctypes.c_double(xl),ctypes.c_double(yl),ctypes.c_double(w),ctypes.c_double(h))

def G_fill_rectangle(xl,yl,w,h):
    w = FPTlib.Fill_Rectangle_X(ctypes.c_double(xl),ctypes.c_double(yl),ctypes.c_double(w),ctypes.c_double(h))
    
def G_triangle(x1,y1, x2,y2, x3,y3):
    w = FPTlib.Triangle_X(ctypes.c_double(x1),ctypes.c_double(y1),ctypes.c_double(x2),ctypes.c_double(y2),ctypes.c_double(x3),ctypes.c_double(y3))

def G_fill_triangle(x1,y1, x2,y2, x3,y3):
    w = FPTlib.Fill_Triangle_X(ctypes.c_double(x1),ctypes.c_double(y1),ctypes.c_double(x2),ctypes.c_double(y2),ctypes.c_double(x3),ctypes.c_double(y3))    

def G_circle(x,y,r):
    w = FPTlib.Circle_X(ctypes.c_double(x),ctypes.c_double(y),ctypes.c_double(r))

def G_fill_circle(x,y,r):
    w = FPTlib.Fill_Circle_X(ctypes.c_double(x),ctypes.c_double(y),ctypes.c_double(r))    
    
def G_polygon(x,y):
    lx = len(x)
    ly = len(y)
    if lx < ly :
        n = lx
    else :
        n = ly
        
    u = (c_double*n)() # creates an array of 5 doubles
    v = (c_double*n)() # creates an array of 5 doubles
    jcopy(u,x, n)
    jcopy(v,y, n)    
    w = FPTlib.Polygon_DX(u,v,  ctypes.c_double(n)) # Polygon_DX actually wants the number of items passed in as a double

def G_fill_polygon(x,y):
    lx = len(x)
    ly = len(y)
    if lx < ly :
        n = lx
    else :
        n = ly
        
    u = (c_double*n)() # creates an array of 5 doubles
    v = (c_double*n)() # creates an array of 5 doubles
    jcopy(u,x, n)
    jcopy(v,y, n)    
    w = FPTlib.Fill_Polygon_DX(u,v,  ctypes.c_double(n))    
    
def G_wait_key():
    w = FPTlib.G_wait_key()
    return w

def G_no_wait_key():
    w = FPTlib.G_no_wait_key()
    return w

def G_wait_click(p):
    u = (c_double*2)() # creates an array of 2 doubles
    w = FPTlib.G_wait_click(u)
    # the caller had better make certain p has room for at least two values :
    p[0] = u[0]
    p[1] = u[1]

def G_wait_mouse():
    w = FPTlib.G_wait_mouse()

def G_x_mouse():
    FPTlib.G_x_mouse.restype = ctypes.c_double
    w = FPTlib.G_x_mouse()    
    return w

def G_y_mouse():
    FPTlib.G_y_mouse.restype = ctypes.c_double  
    w = FPTlib.G_y_mouse()    
    return w  

  

def G_save_image_to_file(fname):
    w = FPTlib.Save_Image_To_File_X(bytes(fname.encode()))
    
def G_save_to_bmp_file(fname):
    w = FPTlib.G_save_to_bmp_file(bytes(fname.encode()))

def G_draw_string(strname, x, y):
    w = FPTlib.Draw_String_X(bytes(strname.encode()), ctypes.c_double(x), ctypes.c_double(y))    

def G_close():
    w = FPTlib.Close_Down_X()

def G_display_image():
    w = FPTlib.Copy_Buffer_And_Flush_X()

def get_time(hms):
    u = (c_int*3)() # creates an array of 3 ints
    w = FPTlib.get_timeI(u)
    # the caller had better make certain hms has room for at least two values :
    hms[0] = u[0]
    hms[1] = u[1]
    hms[2] = u[2]
    return w

  
def G_get_pixel(x,y):
    w = FPTlib.Get_Pixel_X(ctypes.c_double(x),ctypes.c_double(y))
    return w    
    


    
