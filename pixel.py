import cv2
import matplotlib.pyplot as plt
import numpy as np
# img = cv2.imread("background_ps.png")
# plt.imshow(img)
# plt.scatter(302, 228)
# plt.scatter(839, 228)
# xs = np.arange(302, 840, 44.75)
# ys = np.ones_like(xs)*228
# plt.scatter(xs, ys)
# print((839-302)/12)
# plt.show()
lin = np.arange(-10, 10, 1)
boundary = 4
chessX, chessY = np.meshgrid(lin, lin)
chessX = chessX.reshape(-1)
chessY = chessY.reshape(-1)
# arg = ((chessX <= boundary) & (chessY >= -boundary) & (chessY <= chessX+boundary)
#        ) | ((chessX >= -boundary) & (chessY <= boundary) & (chessY >= chessX-boundary))
arg = ((chessX+chessY <= boundary) & (chessX >= -boundary) & (chessY >= -boundary)
       ) | ((chessX <= boundary) & (chessY <= boundary) & (chessX+chessY >= -boundary))

plt.scatter(chessX[arg], chessY[arg])

cnt = 0
spawn = [[], [], [], [], [], []]
for x, y in zip(chessX[arg], chessY[arg]):
    # print(f',{x},{y}', end="")
    cnt += 1
    if y < -boundary:
        spawn[0].append((x, y))
    elif x > boundary:
        spawn[1].append((x, y))
    elif x+y > boundary:
        spawn[2].append((x, y))
    elif y > boundary:
        spawn[3].append((x, y))
    elif x < -boundary:
        spawn[4].append((x, y))
    elif x+y < -boundary:
        spawn[5].append((x, y))
print(spawn[2])
for sp in spawn:
    x, y = np.array(sp).T
    for s, t in zip(x, y):
        # print(f',{s},{t}', end="")
        print(f',{s+t},{-t}', end="")
    print("")
u = np.array([1, 0])
v = np.array([0.5, np.sqrt(3)/2])
x, y = np.dot(np.c_[u, v], np.c_[chessX, chessY].T)
# print(x, y)
plt.scatter(x[arg], y[arg])
# plt.scatter(chessX[arg], chessY[arg])
plt.show()
