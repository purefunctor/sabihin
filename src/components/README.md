# Notes on Components

### `AuthForm`

`react-reason` / `server-react-reason` diverges in that the latter uses `method_` instead of `method` for the `form` element. `AuthForm` needs to be copied over into the `js` folder and modified such that it uses the right prop name for the right platform.
