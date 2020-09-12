#!/usr/bin/env python



import angr
import claripy


def main():
    project = angr.Project("./guest")

    #create an initial state with a symbolic bit vector as argv1
    argv1 = claripy.BVS("argv1",100*8) #since we do not the length now, we just put 100 bytes
    initial_state = project.factory.entry_state(args=["./crackme1",argv1])

    #create a path group using the created initial state 
    sm = project.factory.simulation_manager(initial_state)

    #symbolically execute the program until we reach the wanted value of the instruction pointer
    sm.explore(find=0x4005c1) #at this instruction the binary will print(the "correct" message)

    found = sm.found[0]
    #ask to the symbolic solver to get the value of argv1 in the reached state as a string
    solution = found.solver.eval(argv1, cast_to=bytes)

    print(repr(solution))
    solution = solution[:solution.find(b"\x00")]
    print(solution)
    return solution


if __name__ == '__main__':
    print(repr(main()))

