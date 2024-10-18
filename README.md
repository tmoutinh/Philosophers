# Philosophers

## Table of Contents
- [Project Overview](#project-overview)
- [Technologies Used](#technologies-used)
- [Lessons Learned](#lessons-learned)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [License](#license)

## Project Overview
The Philosophers project is an implementation of the classic synchronization problem known as the "Dining Philosophers Problem." The project aims to demonstrate techniques for managing concurrent processes and preventing deadlocks.

## Technologies Used
- **C**: 92.3%
- **Makefile**: 7.7%

## Lessons Learned
Throughout the development of this project, I have learned:
- **Concurrency Control**: Techniques for managing multiple threads and processes.
- **Synchronization Mechanisms**: Using mutexes and semaphores to avoid race conditions and deadlocks.
- **Problem-Solving**: Implementing solutions for classical computer science problems.
- **C Programming**: Enhancing skills in C programming and using Makefiles for project compilation.

## Getting Started
To get a local copy up and running, follow these simple steps:

1. **Clone the repo**
   ```sh
   git clone https://github.com/tmoutinh/Philosophers.git
   ```
2. **Compile the project**
   ```sh
   cd Philosopher/philo && make
   ```
## Usage
 - Run the program with the following command:
   ```sh
   ./philosophers [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
   ```

## License
Distributed under the MIT License. See LICENSE for more information.
