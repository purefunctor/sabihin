# Notes on Components

## `AuthCore`

The `AuthCore` module implements a generic component used to define `Login.re` and `Register.re` forms. It accepts several optional props that are provided by the client but can be ignored by the server. More specifically, these props are used for implementing client-side behaviour such as hooks to `onChange` or `onSubmit`.

It also implements an abstraction for field validation: `FieldState`, and `MakeField`.

`FieldState` defines an interface for "stateful" fields; for example, a password field may have several states representing its strength. These states can then be rendered into styles that can be added to the field, as well as into helpful messages that can be displayed.

`MakeField` is a functor that consumes a `FieldState` module and creates a React component representing that field.
