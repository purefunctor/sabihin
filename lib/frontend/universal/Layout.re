let divCss = [%cx {|
flex-grow: 1;
min-height: 100vh;
|}];

[@react.component]
let make = () => {
  <> <div className=divCss> <Header /> <Router /> </div> <Footer /> </>;
};
