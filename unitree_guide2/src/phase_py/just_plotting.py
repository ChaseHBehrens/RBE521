import matplotlib.pyplot as plt
import numpy as np

import matplotlib.animation as animation
from functools import partial
   
from phase_diagram import test_data_collection, init_phase_diagram, update, read_file, VID_FILEPATH


if __name__ == '__main__':
    n = 4 # legs
    # test_data_collection(n)

    phase_data, y = read_file()
    
    fig, ax = plt.subplots()
    ax = init_phase_diagram(ax, n)
    l = len(phase_data)

    ani = animation.FuncAnimation(fig, partial(update, ax=ax, x=phase_data, y=y, n=n), frames=l, interval=30, blit=False, repeat=False)    
    ani.save(filename=VID_FILEPATH, writer="pillow")
    
    plt.show()