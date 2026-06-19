# RackInsert-C

RackInsert-C is a standalone robotics C/MuJoCo benchmark for a staged robotic
cable inseertion task. It loads a MuJoCO scene (from Google's AIC 
competition), runs a simple staged insertion policy (hardcoded for now), 
evaluates plug-to-socket geometry, scores each trial, and can record 
trajectories for replay.

## Motivation
This project was build as an Imperial C Project Extension to explore robotics
simulation, MuJoCo, finite-state control, and benchmark/evaluator design in C.

The scene assets are sourced from [Intrinsic's AI for Industry Challenge](https://www.intrinsic.ai/events/ai-for-industry-challenge).
The scoring designed is also inspired by the AIC evalouator structure: model
validity, motion quality, and insertion success. The evaluator we built here is
a simplified independent C implementation by us, not the official AIC scorer.

## Features
- Loads MuJoCo MJCF scenes from MuJoCo's C API.
- Uses AIC-derived robot, task-board, cable and TCP assets.
- During demo, runs a staged scripted policy:
    - APPROACH (server rack)
    - HOVER    (hover above cable socket)
    - ALIGN    (aligns plug about socket before inertion)
    - INSERT
- During evaluation, computes plug-tip to socket geometry:
    - plug-port distance
    - lateral error of TCP to socket
    - axial insertion depth of TCP
- Scores trials using 3-tier AIC-inspired score rubric
- Writes CSV traces for post-demo replay (can be used for training Robotic 
  Models, e.g. Action Chunking Transformers)
- Includes unit/smoke tests for evaluators, actuators, and scripted policy.

## Side Note
Personally, I (Shi Hao) decided to build this by hand because I've joined 2 
sim-to-real robotics competition (NVIDIA x Revel && Intrinsic's AIC) where I 
used Gazebo, NVIDIA Isaac Sim, MuJoCO and PyTorch, and during each of them,
the becnhmark/evaluator infrastructure (like this repo) were already build by
the hosts. Which, had left a gap in my understanding, as I never managed to had
the opportunity onto designing and building my own simulation-environment.
This is a crucial part of teaching a robot by leveraging sim-to-real... which is
hence why I decided to tackle this for the C project... I also get to gain 
experience using MuJoCo's C API, as conventionally I'm more used to controlling
MuJoCo, Gazebo, etc. inside Python.

Author: Shi Hao