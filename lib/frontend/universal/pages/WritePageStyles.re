let writeCss = [%cx
  {|
  font-family: "Poppins";
  margin-top: 2rem;
  margin-bottom: 2rem;

  & > h1 {
    margin-top: 0;
  }

  $(Theme.afterMedium) {
    align-self: center;
    width: 100%;
    max-width: 768px;
  }
|}
];

let fieldCss = [%cx {|
  display: flex;
  flex-direction: column;
|}];

let formCss = [%cx
  {|
  display: flex;
  flex-direction: column;
  gap: 1rem;
|}
];

let fieldCommonCss = [%cx
  {|
  color: $(Theme.foreground10);
  background-color: $(Theme.background11);
  border: 2px solid $(Theme.background9);
  border-radius: 4px;

  font-family: "Poppins";
  font-size: 1.2rem;
  padding: 0.5rem;

  &:focus {
    outline: none;
    border-color: $(Theme.primary);
  }
|}
];

let recipientInputCss = fieldCommonCss;

let messageEditableCss =
  fieldCommonCss
  ++ " "
  ++ [%cx
    {|
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  text-align: center;
  min-height: 12rem;
|}
  ];

let messageRemainingCss = [%cx {|
  align-self: end;
|}];

let actionGroupCss = [%cx
  {|
  display: flex;
  justify-content: space-between;
  gap: 1rem;
|}
];

let actionCss = [%cx
  {|
  font-family: "Poppins";
  font-weight: 500;
  display: flex;
  gap: 0.5rem;
|}
];

let clearCss = Theme.ghostButton ++ " " ++ actionCss;
let sendCss = Theme.primaryButton ++ " " ++ actionCss;
