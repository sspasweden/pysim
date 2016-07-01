import sys

import numpy as np
import matplotlib.pyplot as plt

from pysim.simulation import Sim
from pysim.cythonsystem import Sys

from collections import OrderedDict

class VanDerPol(Sys):
    def __init__(self):
        self.add_state("x", "dx", 2)
        self.add_input("k",2)
        self.add_output("out",2)
        self.inputs.k = [1.0, 1.0]
        self.states.x = [1.0, 0.0]

    def do_step(self,time):
        a = self.inputs.k[0]
        b = self.inputs.k[1]
        x = self.states.x[0]
        y = self.states.x[1]

        x1 = a*x*(b-y*y)-y
        x2 = x
        self.ders.dx = [x1,x2]

def main():
    """Test that the elapsed time is returned from the simulation"""
    sys = VanDerPol()
    sys.store("x")
    sim = Sim()

    sim.add_cython_system(sys,"Cython1")
    sim.simulate(20, 0.1)

    plt.plot(sys.res.x[:,0])
    plt.plot(sys.res.x[:,1])
    plt.show()

if __name__ == "__main__":
    sys.exit(int(main() or 0))