
import numpy as np
import cv2


def draw_label(I,l,color=(255,0,0),thickness=1):
	wh = np.array(I.shape[1::-1]).astype(float)
	tl = tuple((l.tl()*wh).astype(int).tolist())
	br = tuple((l.br()*wh).astype(int).tolist())
	cv2.rectangle(I,tl,br,color,thickness=thickness)


def draw_losangle(I,pts,color=(1.,1.,1.),thickness=1):
	assert(pts.shape[0] == 2 and pts.shape[1] == 4)

	#for i in range(4):
	pt1 = tuple(pts[:,0].astype(int).tolist())
	pt2 = tuple(pts[:,1].astype(int).tolist())
        pt3 = tuple(pts[:,2].astype(int).tolist())
        pt4 = tuple(pts[:,3].astype(int).tolist())
        triangle=np.array([pt1,pt2,pt3,pt4])
        cv2.fillConvexPoly(I, triangle, (255, 255, 255))
		#cv2.line(I,pt1,pt2,color,thickness)
        #for n in range(10,175, 15):
            #for j in range(10,100, 15):
                #I[n:n + 15, j:j + 15] = I[n + (15 // 2)][j + (15 // 2)]


def write2img(Img,label,strg,txt_color=(0,0,0),bg_color=(255,255,255),font_size=1):
	wh_img = np.array(Img.shape[1::-1])

	font = cv2.FONT_HERSHEY_SIMPLEX

	wh_text,v = cv2.getTextSize(strg, font, font_size, 3)
	bl_corner = label.tl()*wh_img

	tl_corner = np.array([bl_corner[0],bl_corner[1]-wh_text[1]])/wh_img
	br_corner = np.array([bl_corner[0]+wh_text[0],bl_corner[1]])/wh_img
	bl_corner /= wh_img

	if (tl_corner < 0.).any():
		delta = 0. - np.minimum(tl_corner,0.)
	elif (br_corner > 1.).any():
		delta = 1. - np.maximum(br_corner,1.)
	else:
		delta = 0.

	tl_corner += delta
	br_corner += delta
	bl_corner += delta

	tpl = lambda x: tuple((x*wh_img).astype(int).tolist())

	cv2.rectangle(Img, tpl(tl_corner), tpl(br_corner), bg_color, -1)	
	cv2.putText(Img,strg,tpl(bl_corner),font,font_size,txt_color,3)
