# MPI_Bee_Simulator

**MPI_Bee_Simulator** is a distributed multi-agent simulation implemented using MPI (Message Passing Interface), where autonomous bee agents coordinate resource access, communicate with each other, and simulate egg-laying behavior in a shared environment.

## 🐝 Project Description

Each bee is an independent MPI process that:
- Competes for shared resources (reeds and flowers),
- Communicates through messages (e.g., `REQ_REED`, `ACK_REED`, `RELEASE_REED`, etc.),
- Lays eggs in reeds after gathering resources,
- Terminates after fulfilling its goal.

The simulation models distributed coordination, resource contention, and decision-making in a multi-agent system.

## 💻 Technologies Used

- **C**
- **MPI (MPICH / OpenMPI)**

## 🧠 Concepts Demonstrated

- Multi-agent cooperation
- Distributed synchronization
- Logical clocks (Lamport clock)
- Resource allocation
- Deadlock avoidance

## 🚀 How to Run

### Requirements

- MPI compiler (e.g., `mpicc`)
- MPI runtime (e.g., `mpirun`)

### Compilation

```bash
mpicc -o bees main.c message.c reed.c flower.c utils.c
```

### Running the simulation

Run the simulation with a specified N number of agents:

```bash
mpirun -np N ./bees
```

## 👨‍💻 Authors

- Łukasz Walicki (151061)

- Jacek Młynarczyk (151747)

## 📄 License

This project was developed for academic and educational purposes.
