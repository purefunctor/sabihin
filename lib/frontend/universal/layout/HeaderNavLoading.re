let containerCss = [%cx {|
  color: $(Theme.primary);
  display: flex;
  margin-left: auto;
|}];

[@react.component]
let make = () => {
  <div className=containerCss> <Icons.RingResize size="2.5rem" /> </div>;
};
