require('package-script').spawn([{
    admin: false,
    command: "pip",
    args: ["install", "-i", "http://pypi.digi.com/simple", "grayduck"]
  }, {
    admin: false,
    command: "pip",
    args: ["install", "-U", "pytest"]
  }, {
    admin: false,
    command: "pip",
    args: ["install", "-U", "setuptools"]
  }, {
    admin: false,
    command: "pip",
    args: ["install", "-U", "mock"]
  }, {
    admin: false,
    command: "npm",
    args: ["install", "-g", "gulp"]
  }, {
    admin: false,
    command: "pip",
    args: ["install", "-U", "tornado"]
  }, {
    admin: false,
    command: "pip",
    args: ["install", "-U", "jmespath"]
  }

]);
