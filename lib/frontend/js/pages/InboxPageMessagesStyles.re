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
  grid-template-areas:
    "content content content"
    "trash   reply   share";
  grid-template-rows: 80% 20%;
|}
  ];

let cardFaceCloseCss = [%cx
  {|
  position: absolute;
  top: 0.5rem;
  right: 0.5rem;
  padding: 0;

  border: none;
  background-color: transparent;
  cursor: pointer;
  color: $(Theme.foreground9);

  &:hover {
    color: $(Theme.primary);
  }
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
  grid-area: content;
|}
];

let cardFaceButtonCss = [%cx
  {|
  flex-grow: 1;
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

  transition: color 150ms ease-in, background-color 150ms ease-in;

  &:nth-child(2n) {
    border-left-width: 2px;
    border-right-width: 2px;
  }
|}
];

let cardFaceTrashCss =
  cardFaceButtonCss
  ++ " "
  ++ [%cx
    {|
color: $(Theme.failure);
&:hover {
  color: $(Theme.background9);
  background-color: $(Theme.failure);
}
grid-area: trash;
|}
  ];

let cardFaceReplyCss =
  cardFaceButtonCss
  ++ " "
  ++ [%cx
    {|
color: $(Theme.info);
&:hover {
  color: $(Theme.background9);
  background-color: $(Theme.info);
}
grid-area: reply;
|}
  ];

let cardFaceShareCss =
  cardFaceButtonCss
  ++ " "
  ++ [%cx
    {|
color: $(Theme.primary);
&:hover {
  color: $(Theme.background9);
  background-color: $(Theme.primary);
}
grid-area: share;
|}
  ];

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
