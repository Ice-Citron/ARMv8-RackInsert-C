import argparse
import csv
import pathlib
import time

import mujoco
import mujoco.viewer

DEFAULT_SCENE: str = "assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml"


# (Helper): Handles data plumbing, filters out columns with headers that starts
#       with `qpos_` (recorded joint angles)
def qpos_columns(fieldnames) -> list:
    cols: list = [name for name in fieldnames if name.startswith("qpos_")]
    return sorted(cols, key=lambda name: int(name.split("_", 1)[1]))


# (Helper): Handles data plumbing, by processing CSV in `path`
def load_trace(path: str) -> tuple:
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        rows = list(reader)
        cols = qpos_columns(reader.fieldnames or [])

    if not rows:
        raise RuntimeError("ERROR: Empty trace, no rows found.")
    if not cols:
        raise RuntimeError("ERROR: Trace has no qpos columns.")
    
    return rows, cols


# (Helper): Handles data plumbing. 
def apply_ctrl(model, data, row, cols):
    for i, col in enumerate(cols):
        if i >= model.nu:   # mmodel.nu = number of active control DOF
            break
        data.ctrl[i] = float(row[col])


def apply_qpos(model, data, row, cols):
    for i, col in enumerate(cols):
        if i >= model.nq:   # mmodel.nu = number of angle-cords of robot's DOF
            break
        data.qpos[i] = float(row[col])
    # Computes the physics (kinematics && dynamics) initially with time frozen
    mujoco.mj_forward(model, data)    


# Where the 3D rendering and MuJoCO simulation happens
def replay_view(args):
    model = mujoco.MjModel.from_xml_path(args.scene)
    data = mujoco.MjData(model)
    rows, cols = load_trace(args.trace)

    # Computes the physics (kinematics && dynamics) initially with time frozen
    mujoco.mj_forward(model, data)

    with mujoco.viewer.launch_passive(model, data) as viewer:
        # Nicer initial camera view in MuJoCo sim
        viewer.cam.azimuth = 135
        viewer.cam.elevation = -25
        viewer.cam.distance = 2.2
        viewer.cam.lookat[:] = [0.25, -0.15, 1.1]

        prev_t = float(rows[0]["time"])

        for row in rows:
            if not viewer.is_running():
                break

            cur_t = float(row["time"])
            delta_t= cur_t - prev_t
            delay= max(0.0, delta_t) / args.speed
            prev_t = cur_t

            apply_qpos(model, data, row, cols)
            viewer.sync()   # updates pixels on screen to latest sim-state
            
            # Because CPU can simulate MuJoCo physics 1k+ times faster than
            # reality, `delay` is used to artificially slow down the loop. Such
            # to ensure that users get to wwatch robot mvoe as if in real-time
            if delay > 0.0:
                time.sleep(delay)

        print("Replay finished. Close viewer window to exit.")
        while viewer.is_running():
            viewer.sync()
            time.sleep(0.02)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--trace", required=True)
    parser.add_argument("--scene", default=DEFAULT_SCENE)
    # speed=1.0 for normal speed, 2.0 for 2-times speed, 0.5 for slow-motion
    parser.add_argument("--speed", type=float, default=1.0)
    args = parser.parse_args()

    if not pathlib.Path(args.trace).exists():
        raise RuntimeError("ERROR: Path to recorded trial CSV does not exists.")

    replay_view(args)

if __name__ == "__main__":
    main()
