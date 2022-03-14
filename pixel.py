import cv2
import matplotlib.pyplot as plt
import numpy as np
img = cv2.imread("background_ps.png")
plt.imshow(img)
# plt.scatter(302, 228)
# plt.scatter(839, 228)
xs = np.arange(302, 840, 44.75)
ys = np.ones_like(xs)*228
plt.scatter(xs, ys)
print((839-302)/12)
plt.show()
