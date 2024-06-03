let messagesCss = [%cx
  {|
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  padding-top: 2rem;
  padding-bottom: 2rem;
  gap: 2rem;
|}
];

let cardOuterCss = [%cx
  {|
  display: flex;
  position: relative;
  height: 20rem;
  width: 100%;

  $(Theme.afterMedium) {
    width: calc(50% - 1rem);
  }
|}
];

let cardInnerCss = [%cx
  {|
  height: 100%;
  width: 100%;
  position: absolute;
  backface-visibility: hidden;
  border-radius: 4px;
  box-sizing: border-box;
|}
];

let cardFaceCss =
  cardInnerCss
  ++ " "
  ++ [%cx
    {|
  background-color: $(Theme.background11);
  border-color: $(Theme.background9);
  border-style: solid;
  border-width: 2px;
  color: $(Theme.foreground9);
  overflow: hidden;
  display: grid;
  grid-template-rows: 1fr 4rem;
  grid-template-columns: 4rem 1fr 1fr 1fr;
|}
  ];

let cardFaceContentCss = [%cx
  {|
  font-family: "Poppins";
  font-size: 1.2rem;
  text-align: center;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 2rem;
  grid-row: 1;
  grid-column: 1 / span 4;
|}
];

let cardFaceActionsCss = [%cx {|
  display: flex;
|}];

let cardFaceButtonGroupCss = [%cx
  {|
  flex-grow: 1;
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
|}
];

let cardFaceButtonCss = [%cx
  {|
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 8px;
  padding: 0;

  border-color: $(Theme.background9);
  border-style: solid;
  border-width: 2px 0 0 0;

  background-color: transparent;
  cursor: pointer;
  font-family: "Poppins";
  font-size: 1rem;

  &:not(:last-child) {
    border-right-width: 2px;
  }

  grid-row: 2;
|}
];

let cardFaceMoreCss =
  cardFaceButtonCss
  ++ " "
  ++ [%cx
    {|
  color: $(Theme.foreground11);
  &:hover {
    color: $(Theme.foreground9);
  }
|}
  ];

let makeCardFaceButton = color => {
  cardFaceButtonCss
  ++ " "
  ++ [%cx
    {|
    color: $(color);
    &:hover {
      color: $(Theme.background9);
      background-color: $(color);
    }
    transition:
      color 150ms ease-in,
      background-color 150ms ease-in;
  |}
  ];
};

let cardFaceFailCss = makeCardFaceButton(Theme.failure);
let cardFaceCloseCss = cardFaceFailCss;
let cardFaceReplyCss = makeCardFaceButton(Theme.secondary);
let cardFaceShareCss = makeCardFaceButton(Theme.primary);

let cardBackCss =
  cardInnerCss
  ++ " "
  ++ [%cx
    {|
  background-color: $(Theme.background9);
  background-image: url("static/noise.png");
  border-radius: 4px;
  color: $(Theme.primary);
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
|}
  ];
