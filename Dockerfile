# Use the official GCC image from the Docker Hub
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Compile the C++ program
RUN g++ -o myapp main.cpp

# Command to run the executable
CMD ["./myapp"]