# Contributing to Golden Audio JUCE

Thank you for considering contributing to Golden Audio JUCE! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Bugs

- Check if the bug has already been reported in [Issues](https://github.com/yourusername/golden-audio-juce/issues)
- Use the bug report template
- Include:
  - OS and version
  - JUCE version
  - CMake version
  - Minimal reproducible example
  - Expected vs actual behavior

### Suggesting Features

- Check existing feature requests first
- Clearly describe the use case
- Explain how it benefits users
- Consider backward compatibility

### Pull Requests

1. **Fork** the repository
2. **Create a branch** from `main`:
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make your changes**:
   - Follow existing code style
   - Add tests if applicable
   - Update documentation
   - Keep commits atomic and well-described

4. **Test your changes**:
   ```bash
   cmake -B build -S .
   cmake --build build
   ```

5. **Submit PR**:
   - Reference related issues
   - Describe changes clearly
   - Include examples if adding features

## Code Style

- **C++17** standard
- **4 spaces** for indentation (no tabs)
- **CamelCase** for classes: `GoldenAudioTestCase`
- **camelCase** for methods: `addParameter()`
- **snake_case** for member variables: `sample_rate_`
- Use `const` and `override` appropriately
- Document public APIs with comments

## Testing

- Test with multiple JUCE versions (7.0+)
- Test on Windows, macOS, and Linux if possible
- Verify examples still build
- Check that header-only design is maintained

## Documentation

- Update README.md if adding features
- Update CHANGELOG.md following [Keep a Changelog](https://keepachangelog.com/)
- Add inline documentation for public APIs
- Update examples if API changes

## Commit Messages

Format: `type: description`

Types:
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation only
- `refactor:` Code restructuring
- `test:` Adding tests
- `chore:` Maintenance

Example:
```
feat: add support for custom MIDI CC automation
```

## Code of Conduct

- Be respectful and constructive
- Welcome newcomers
- Focus on technical merit
- Assume good intentions

## Questions?

Open a [Discussion](https://github.com/yourusername/golden-audio-juce/discussions) for questions about contributing.

Thank you for improving Golden Audio JUCE! 🎵
