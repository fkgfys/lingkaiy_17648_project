# API Server Container

This project containerizes a simple API server written in C using [libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/). The server generates a JSON response containing vehicle data and listens on port 8080. It runs inside an Ubuntu container.

## Prerequisites

- [Docker](https://docs.docker.com/get-docker/)
- [Docker Compose](https://docs.docker.com/compose/install/)

## Project Files

- **vapi.c**: The C source file for the API server.
- **Dockerfile**: Builds an Ubuntu-based container, installs build tools, compiles `vapi.c`, and runs the API server.
- **docker-compose.yml**: Defines the service for the API server and maps port 8080.

## Building and Running the Container

1. **Clone the Repository**

    Clone this repository and navigate to its directory:
    ```bash
    git clone https://github.com/fkgfys/lingkaiy_17648_project.git
    cd lingkaiy_17648_project
    ```

2. **Build and Start the Contianer**

    ```bash
    docker-compose build
    docker-compose up -d
    ```

3. **Call the API**
    ```bash
    curl -X GET http://localhost:8080/data
    ```

4. **Understanding the API Response**
    ```json
    {
        "Oil Temperature": 197,
        "MAF Sensor": 19,
        "Battery Voltage": 9,
        "Tire Pressure": 491,
        "Fuel Level": 85,
        "Fuel Comsumption Rate": 5,
        "Error Code": "C7"
    }
    ```
    **Oil Temperature:** Degrees F. \
    **MAF Sensor:** 0-11 bit A/D signal \
    **Battery Voltage:** 0-12 volts (universal across all vehicles) \
    **Tire Pressure:** 0-11 bit A/D signal \
    **Fuel Level:** Liters (max is vehicle specific) \
    **Fuel Consumption Rate:** Liters/hour \
    **Error Codes:** Set of 4 8-bit universal codes, unordered
    
    | Code (Hex)  | Description |
    | ------------- | ------------- |
    | 0xA1 – 0xA8   | Misfire on Cylinder 1-8 (0xA1 = cyl. 1, 0xA2 = cyl. 2...)  |
    | 0xC1 – 0xC8  | Fuel Injector Error on cylinder (0xC1 = inj. 1, 0xC2 = inj. 2...)  |
    | 0X55  | Low Oil Pressure  |
    | 0x23  | Low Coolant Level  |