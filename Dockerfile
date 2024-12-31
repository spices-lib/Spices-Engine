# Use Node.js 22
FROM node:22

# Update and install doxygen 和 graphviz
RUN apt-get update && \
   apt-get install -y doxygen graphviz && \
   apt-get clean && \
   rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy all files
COPY . .

# Install npm dependencies
RUN npm install

# Build the solution docuemnts
RUN bash ./Scripts/GenerateDocuments.sh