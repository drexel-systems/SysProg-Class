# Grading C Code for Quality

When tasked with grading C code for quality, use these intructions:

## Naming conventions

- Use descriptive names for variables, functions, and constants. 
- Follow camelCase for most identifiers, with consistent prefixes for specific types (e.g., "g_" for global variables). 

## Readability

- Consistent indentation (e.g., 4 spaces) 
- Proper brace placement 
- Meaningful comments explaining complex logic 
- Line breaks to separate code blocks 

## Data types and casting

- Use appropriate data types (int, float, char, etc.) 
- Avoid unnecessary casting 
- Use explicit type conversions when required 

## Function design

- Keep functions small and focused on a single task 
- Use meaningful parameter names 
- Document function purpose and expected behavior 

## Error handling

- Validate input parameters 
- Check for potential errors (e.g., null pointers) 
- Implement proper error handling mechanisms (e.g., return codes, exceptions) 

## Memory management

- Allocate and deallocate memory properly 
- Avoid memory leaks 
- Use dynamic memory allocation carefully 

## Code organization

- Include guards to prevent multiple header file inclusions 
- Separate code into logical modules 
- Use namespaces appropriately (if applicable) 

## Security considerations

- Validate user input to prevent buffer overflows 
- Sanitize data before processing 
- Be aware of potential security vulnerabilities 