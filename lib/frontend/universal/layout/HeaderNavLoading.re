let containerCss = [%cx {|
  color: $(Theme.primary);
  margin-left: auto;
|}];

[@react.component]
let make = () => {
  <div className=containerCss> <Icons.RingResize size="2rem" /> </div>;
};
