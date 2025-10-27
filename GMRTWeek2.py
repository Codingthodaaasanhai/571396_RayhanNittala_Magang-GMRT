import math
import numpy as np

L1 = 96.0   
L2 = 73.0   
theta1_deg = 40.0
theta2_deg = 30.0
theta1 = math.radians(theta1_deg)
theta2 = math.radians(theta2_deg)


x = L1 * math.cos(theta1) + L2 * math.cos(theta1 + theta2)
y = L1 * math.sin(theta1) + L2 * math.sin(theta1 + theta2)

print(f"End-effector position: x ={x:.2f} y ={y:.2f}")


def Rotational(theta):
    return np.array([
        [math.cos(theta), -math.sin(theta), 0],
        [math.sin(theta),  math.cos(theta), 0],
        [0,  0,  1]
    ])

def Translational(d):
    return np.array([
        [1, 0, d],
        [0, 1, 0],
        [0, 0, 1]
    ])

T0_1 = np.dot(Rotational(theta1), Translational(L1))
T1_2 = np.dot(Rotational(theta2), Translational(L2))
T0_2 = np.dot(T0_1, T1_2)

end_pos= np.dot(T0_2, np.array([0, 0, 1]))
x_end, y_end = end_pos[0], end_pos[1]
print("End-effector position: x =", x_end, "y =", y_end)
