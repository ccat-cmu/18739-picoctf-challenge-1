# Unlock Canary CTF Challenges

This repository provides a Binary CTF challenge which leverages format string vulnerability bypass mitigation, specifically Canary, and take over the control flow. 

To reduce complexity and unpredictability, the challenge uses a customize stack Canary implementation as a local variable which has a stable value across runs. 

This challenge requires understanding of stack layout, memory alignment and parameter location on a 64-bit machine. C source code and executable are provided to player for necessary information. 

A python script is provided to solve the challenge on an ARM64 environment. For other architecture, simply update the parameter offset. 
