# Project B - CSV Database & Query Engine

## Project Overview

This project implements a CSV database and query engine, providing a robust solution for parsing, indexing, and querying CSV data efficiently.

## Features

### CSV Parser

- **Custom Parser**: A hand-built CSV parser that handles edge cases such as:
    - Quoted fields with embedded delimiters
    - Multi-line values within quotes
    - Escaped characters
    - Empty fields and trailing newlines
- **Data Integrity**: Validates data types, handles missing values gracefully, and provides error reporting for malformed rows

### Library Integration

- **vcpkg/Conan Support**: Optionally integrate lightweight parsing libraries (e.g., CSV parser libraries) via vcpkg or Conan for enhanced performance and reduced maintenance burden
- **Comparison**: Benchmark custom implementation against library solutions

### Index System

- **Primary Index**: Fast lookup by key columns
- **Secondary Indexes**: Support for additional columns to optimize common query patterns
- **B-Tree/Hash Indexes**: Efficient data structures for range and equality queries

### Query Grammar

- **SQL-like Syntax**: Support for basic query operations:
    - `SELECT` columns
    - `WHERE` conditions (equality, comparison operators)
    - `ORDER BY` sorting
    - `LIMIT` results
- **Parser**: Custom lexer and parser to interpret query commands

## Architecture

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   Query     │────▶│   Query     │────▶│   Result    │
│   Input     │     │   Engine    │     │   Output    │
└─────────────┘     └─────────────┘     └─────────────┘
                                                     │
                ┌──────────────────┼──────────────────┐
                ▼                  ▼                  ▼
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│    CSV      │    │   Index     │    │  Execution  │
│   Parser    │    │   Manager   │    │   Planner   │
└─────────────┘    └─────────────┘    └─────────────┘
```

## Building the Project

```bash
# Using CMake
mkdir build && cd build
cmake ..
make

# Using vcpkg (optional)
vcpkg install csv-parser
```

## Usage

```bash
# Load a CSV file
./csvdb --load data.csv

# Execute a query
./csvdb --query "SELECT * FROM data WHERE age > 25"

# Create an index
./csvdb --index data.csv --column name
```

## Performance Comparison

| Aspect | Custom Parser | Library Parser |
|--------|---------------|----------------|
| Speed | Baseline measurement | Optimized implementation |
| Memory | Direct memory control | Library overhead |
| Maintenance | Full control | External dependency |

## Testing

Run unit tests to verify parser correctness and query engine functionality:

```bash
./test_runner
```

## Deliverables

- [ ] Reliable CSV parser with edge case handling
- [ ] Index system for fast data access
- [ ] Query grammar implementation
- [ ] Performance comparison report
