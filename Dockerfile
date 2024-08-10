# Stage 1: Build
FROM gcc:latest AS builder

# Set the working directory in the container
WORKDIR /app

# Copy the source code to the container
COPY . .

# Install necessary packages
RUN apt-get update && \
    apt-get install -y make

# Compile the C++ application
RUN g++ -o http_server main.cpp

# Stage 2: Runtime
FROM ubuntu:24.10

# Set the working directory in the container
WORKDIR /app

# Copy the compiled binary from the builder stage
COPY --from=builder /app/http_server /app/http_server

# Expose the port the application will run on
EXPOSE 8080

# Run the application
CMD ["./http_server"]
