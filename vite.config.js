import { defineConfig } from "vite";

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
  plugins: [],
});
