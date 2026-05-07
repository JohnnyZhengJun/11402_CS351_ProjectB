# Project B - CSV Database & Query Engine

## Project Overview

This repository implements a CSV database and query engine with a focus on the query execution code in `QE.cpp`. The system supports loading CSV data, building indexes, parsing SQL-like queries, and executing filters, projections, sorting, and limits.

## Features

### CSV Parser

- **Custom Parser**: Handles CSV edge cases such as:
  - Quoted fields with embedded delimiters
  - Multi-line values within quotes
  - Escaped characters
  - Empty fields and trailing newlines
- **Data Integrity**: Validates types, handles missing values, and reports malformed rows

### Query Execution Engine

- **SQL-like Syntax**: Supports basic query statements:
  - `SELECT` columns
  - `WHERE` conditions (equality, comparison operators)
  - `ORDER BY` sorting
  - `LIMIT` results
- **QE.cpp Implementation**: Implements query planning, predicate evaluation, row projection, and result generation

### Index System

- **Primary Index**: Fast lookup by key columns
- **Secondary Indexes**: Optimize common query patterns on other columns
- **B-Tree/Hash Indexes**: Designed for efficient range and equality queries

## Architecture

```text
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
g++ QE.cpp -o QE
```

## Usage

```bash
# Load a CSV file
./QE --csv PATH_file
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
mini-db> SELECT * FROM table_name
```

## Deliverables

- [ ] Reliable CSV parser with edge case handling
- [ ] Index system for fast data access
- [ ] Query grammar implementation
- [ ] Performance comparison report
