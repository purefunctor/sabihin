let containerCss = [%cx {|
  margin-left: auto;
|}];

[@react.component]
let make = () => {
  <div className=containerCss> {React.string("You're logged in!")} </div>;
};
