# Contributing to AC Web Remote

Thank you for your interest in contributing to AC Web Remote! This document provides guidelines for contributing to the project.

## Development Setup

### Prerequisites
- [PlatformIO Core](https://platformio.org/install/cli) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- ESP32 development board
- IR LED connected to GPIO 33

### Getting Started
1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/ACWebRemote.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes
6. Commit your changes: `git commit -m "Add your feature"`
7. Push to your fork: `git push origin feature/your-feature-name`
8. Create a Pull Request

## Code Style

### C++ Guidelines
- Use 4 spaces for indentation
- Follow the existing code style in the project
- Add comments for complex logic
- Keep functions focused and small
- Use meaningful variable and function names

### File Organization
- `src/main.cpp` - Main application logic
- `src/config.h` - Configuration constants
- `src/web_interface.cpp` - Web interface generation
- `lib/IRTadiran/` - IR library implementation

## Testing

### Before Submitting
1. Ensure your code compiles: `pio run`
2. Test on actual hardware if possible
3. Test the web interface on different devices
4. Verify WiFi setup and AC control functionality

### Hardware Testing
- Test WiFi configuration portal
- Verify AC commands work with your Tadiran unit
- Check web interface responsiveness
- Test mDNS resolution

## Pull Request Guidelines

### Pull Request Checklist
- [ ] Code compiles without errors
- [ ] Functionality tested on hardware
- [ ] Documentation updated if needed
- [ ] Commit messages are clear and descriptive
- [ ] No hardcoded credentials or sensitive data
- [ ] Follows existing code style

### Commit Messages
Use clear, descriptive commit messages:
- `feat: add temperature scheduling`
- `fix: resolve WiFi reconnection issue`
- `docs: update installation instructions`
- `refactor: simplify web interface generation`

## Bug Reports

When reporting bugs, please include:
- ESP32 board type and version
- PlatformIO version
- Detailed steps to reproduce
- Expected vs actual behavior
- Serial output if applicable
- Screenshots for web interface issues

## Feature Requests

For new features:
- Check existing issues first
- Clearly describe the feature
- Explain the use case
- Consider backward compatibility
- Discuss implementation approach

## Documentation

Help improve documentation by:
- Fixing typos and grammar
- Adding missing information
- Improving clarity
- Adding examples
- Updating screenshots

## Code of Conduct

- Be respectful and inclusive
- Focus on constructive feedback
- Help others learn and grow
- Keep discussions technical and on-topic

## Questions?

- Check the README for common issues
- Search existing issues
- Create a new issue for questions
- Be specific and provide context

Thank you for contributing! 🚀
