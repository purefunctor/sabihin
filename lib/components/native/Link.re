[@react.component]
let make = (~href, ~className, ~children) => {
  <a
    href
    className
    onClick={event => {
      React.Event.Mouse.preventDefault(event);
      ReasonReactRouter.push(href);
    }}>
    children
  </a>;
};
