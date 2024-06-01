let containerCss = [%cx
  {|
  display: flex;
  align-items: center;
  margin-left: auto;
  gap: 2rem;
|}
];

module Medium = HeaderNavLoggedInMedium;
module Small = HeaderNavLoggedInSmall;

[@react.component]
let make = () => {
  <div className=containerCss> <Medium /> <Small /> </div>;
};
