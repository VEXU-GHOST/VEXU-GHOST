# Description
This repository contains code intended to support VEXU and VEXAI teams interested in leveraging advanced programming techniques on their competition robots.

# Setup

The recommended flow is Docker — works the same on macOS, Windows, and Linux, and you never have to match a specific Ubuntu version on the host.

**Windows / macOS users:** start at [SetupMyEnvironment.md](SetupMyEnvironment.md) for Docker Desktop + SSH/Git setup, then come back here.


## 2. Clone the repo (on your host, not inside a container)

```bash
git clone git@github.com:VEXU-GHOST/VEXU-GHOST.git
cd VEXU-GHOST
git submodule update --init --recursive
```

SSH keys and `~/.gitconfig` on your host are bind-mounted read-only into the container, so git inside the container uses the same identity.

## 3. Build the image and start the dev container

> [!NOTE]
> Recommended for most users: open this repo in VS Code and use **Dev Containers: Reopen in Container**. VS Code will manage container start/attach for you and auto-install the recommended extensions from [.devcontainer/devcontainer.json](.devcontainer/devcontainer.json). Use the CLI `docker compose ...` flow below if you prefer terminal-only control.


```bash
docker compose build              # one-time; ~10–15 min on first build
docker compose up -d              # start the long-lived dev container
docker compose exec vexu bash     # open a shell (repeat for more terminals)
```

Inside the shell:

```bash
./scripts/build.sh                # compile the ROS 2 workspace
./scripts/launch_sim.sh           # start the Gazebo sim, watch it at http://localhost:8080/vnc.html
```

When you're done:

```bash
docker compose stop               # pause the container; keep in-container filesystem state
```

`docker compose exec` reuses the same container — background processes like `gz sim` started in one shell stay alive for other shells. `up -d` also starts a noVNC service for browser-based RViz/Gazebo on [http://localhost:8080/vnc.html](http://localhost:8080/vnc.html); Linux users with a native display skip it via `docker compose up -d vexu`.

Use `docker compose down` only when you want a reset: it removes containers and drops writable container filesystem changes (for example, tools/extensions installed inside the container). Bind-mounted repo files and named volumes still persist unless you also pass `--volumes`.

See [12_Docker/README.md](12_Docker/README.md) for GUI, NVIDIA, PROS, dev-container, and implementation details.

# Native Ubuntu 22.04 (alternative)

If you prefer a native install on Ubuntu 22.04 (no Docker), the old manual flow still works:

<details>
<summary>Click to expand native setup</summary>

### Install ROS 2 Humble

Follow: <https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html>

### Repo setup

```bash
cd
git clone git@github.com:VEXU-GHOST/VEXU-GHOST.git
cd VEXU-GHOST
git submodule update --init --recursive
git checkout develop # change develop to the branch you are working on
```

### Add setup to ~/.bashrc

```bash
echo "export VEXU_HOME=\"$HOME/VEXU-GHOST\"" >> ~/.bashrc
echo 'source "$VEXU_HOME/scripts/setup_env.sh"' >> ~/.bashrc
```

Open a new terminal to pick up the new env.

### Build

```bash
./scripts/update_dependencies.sh
./scripts/setup_submodules.sh
./scripts/build.sh
./scripts/launch_sim.sh
```

### Real robot USB access

```bash
sudo usermod -a -G dialout $USER
```

</details>

## Foxglove connection

The example hardware, competition hardware, and example simulation launches start
one Foxglove Bridge automatically on port 8765. Do not start another bridge on that
port alongside them. Native setup installs `ros-humble-foxglove-bridge` through
`scripts/update_dependencies.sh`; the Docker image installs it with apt as well.
The three launch packages also declare it as a runtime dependency for rosdep.

After sourcing your normal environment, the repository CLI is available:

```bash
source "$VEXU_HOME/scripts/setup_env.sh"
ghost foxglove-url            # ws://orinx.local:8765
ghost foxglove-url localhost  # ws://localhost:8765 for local simulation
```

You can also run `./scripts/ghost foxglove-url` directly from the repository.
In Foxglove, select Open connection, choose Foxglove WebSocket, and paste the URL.
The command prints an address; it does not start the bridge or test connectivity.
`orinx.local` must resolve to the robot computer, and your laptop must be able to
reach its port 8765. An alternative hostname or IPv4 address can be passed as the
optional host argument. This does not configure the robot hostname or mDNS.

For local simulation:

```bash
./scripts/launch_sim.sh
# Headless alternative for diagnostics:
./scripts/launch_sim.sh sim_gui:=false rviz:=false
```

Docker publishes port 8765 to the host; recreate the dev container after changing
Compose configuration, and rebuild its image after dependency changes.
ROS runs inside the container, while Foxglove can run on the host.

Before the physical Baby Jerry test, stop any manual bridge, restart the sim with
only its normal launch, and verify that Foxglove receives ROS topics. A 3D panel can
use `/robot_description`, `/tf`, and `/tf_static`; sensor panels require the
corresponding sensor topics to be published. Gazebo must be running for physics.
The physical Baby Jerry test is intentionally deferred until after the next task.
