import numpy as np
import matplotlib.pyplot as plt	
data=np.loadtxt ("CSpoints.txt")
plt.plot(data[:,0], data[:,1])
data=np.loadtxt ("CSknots.txt")
plt.plot(data[:,0], data[:,1], 'go')
plt.minorticks_on()
plt.grid(color="blue", which="major", linewidth=1)
plt.grid(color="blue", which="minor", linestyle=":", linewidth=0.5)
plt.show()
	

