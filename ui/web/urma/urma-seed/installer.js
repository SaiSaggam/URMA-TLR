require('package-script').spawn([
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "gulp"]
    },
    {
        admin: false,
        command: "npm",
        args: ["install", "", "gulp"]
    },
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "meteor-build-client"]
    },
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "mgp"]
    },
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "phantomjs"]
    },
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "spacejam"]
    },
    {
        admin: true,
        command: "npm",
        args: ["install", "-g", "starrynight"]
    }

]);
