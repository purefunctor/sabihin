import { defineConfig } from "vite";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default defineConfig({
  build: {
    manifest: true,
    rollupOptions: {
      input: "index.js"
    },
  },
  server: {
    origin: "http://localhost:8080",
  },
  plugins: [nodeResolve()],
});
