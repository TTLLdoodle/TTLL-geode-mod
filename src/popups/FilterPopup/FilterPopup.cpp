#include "./FilterPopup.hpp"

FilterPopup* FilterPopup::create() {
	auto ret = new FilterPopup();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

constexpr const char* filterWarning =
"Many features in this filter request data from <cy>RobTop servers</c>. The game's servers automatically set usage limits."
"To avoid a temporary ban(up to 1 hour), please avoid updating too frequently."
"Click <cr>I confirm</c> to continue, confirming your understanding of this limitation.";

constexpr const char* filterInfo =
"<cc>The difficulty filter</c> allows you to select levels in the top 50/150/300, Unbounded, or your range.\n"
"<cc>The length filter</c> allows you to select lengths up to 30/60/120/>120, or your range.\n"
"The <cg>rated</c> and <cr>unrated</c> <cc>checkboxes</c> allow you to display only the corresponding levels.\n"
"The <cg>Completed</c> <cc>checkbox</c> only has an effect if the <cf>Username</c> field is not empty and displays the player's completions.\n"
"The <cd>Creator</c> <cc>checkbox</c> only has an effect if the <cf>Creator</c> field is not empty and displays levels from this creator.";

bool FilterPopup::init() {
	if (!Popup::init(450.0f, 280.0f)) return false;

	this->setID("filter-menu");
	this->setZOrder(100);
	this->setTitle("Search filters");

	auto& displayFilters = GlobalList::Filters::getDisplayFilters();

	auto optionsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	optionsSpr->setContentSize({ 50.5f, 50.5f });
	optionsSpr->setScale(0.46);


	auto diffFilterBG = CCScale9Sprite::create("square02b_001.png");
	diffFilterBG->setColor({ 123, 68, 41 });
	diffFilterBG->setContentSize({ 365.0f, 35.0f });
	diffFilterBG->setPosition({ 225.0f, 225.0f });
	m_mainLayer->addChild(diffFilterBG);

	auto diffFilterMenu = CCMenu::create();
	diffFilterMenu->setLayout(RowLayout::create()
		->setGap(16.0f)
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false));
	diffFilterMenu->setContentSize({ 340.0f, 35.0f });
	diffFilterMenu->setPosition({ 225.0f, 225.0f });
	diffFilterMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(diffFilterMenu);

	auto demonSpr = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
	demonSpr->setContentSize({ 26.5f, 26.5f });
	demonSpr->setScale(0.90f);
	diffFilterMenu->addChild(demonSpr);

	m_top50Label = CCLabelBMFont::create("Top 50", "bigFont.fnt");
	m_top50Label->setScale(0.36f);
	if (displayFilters.diffFilter[0]) m_top50Label->setColor({ 255, 255, 255 });
	else m_top50Label->setColor({ 125, 125, 125 });
	auto top50Btn = CCMenuItemExt::createSpriteExtra(m_top50Label, [this](auto) {
		changeDiffFilter(DifficultyFilter::Top50, m_top50Label);
	});
	diffFilterMenu->addChild(top50Btn);

	m_top150Label = CCLabelBMFont::create("Top 150", "bigFont.fnt");
	m_top150Label->setScale(0.36f);
	if (displayFilters.diffFilter[1]) m_top150Label->setColor({ 255, 255, 255 });
	else m_top150Label->setColor({ 125, 125, 125 });
	auto top150Btn = CCMenuItemExt::createSpriteExtra(m_top150Label, [this](auto) {
		changeDiffFilter(DifficultyFilter::Top150, m_top150Label);
	});
	diffFilterMenu->addChild(top150Btn);

	m_top300Label = CCLabelBMFont::create("Top 300", "bigFont.fnt");
	m_top300Label->setScale(0.36f);
	if (displayFilters.diffFilter[2]) m_top300Label->setColor({ 255, 255, 255 });
	else m_top300Label->setColor({ 125, 125, 125 });
	auto top300Btn = CCMenuItemExt::createSpriteExtra(m_top300Label, [this](auto) {
		changeDiffFilter(DifficultyFilter::Top300, m_top300Label);
	});
	diffFilterMenu->addChild(top300Btn);

	m_unboundedLabel = CCLabelBMFont::create("Unbounded", "bigFont.fnt");
	m_unboundedLabel->setScale(0.36f);
	if (displayFilters.diffFilter[3]) m_unboundedLabel->setColor({ 255, 255, 255 });
	else m_unboundedLabel->setColor({ 125, 125, 125 });
	auto unboundedBtn = CCMenuItemExt::createSpriteExtra(m_unboundedLabel, [this](auto) {
		changeDiffFilter(DifficultyFilter::Unbounded, m_unboundedLabel);
	});
	diffFilterMenu->addChild(unboundedBtn);

	m_customDiffLabel = CCLabelBMFont::create("Custom", "bigFont.fnt");
	m_customDiffLabel->setScale(0.36f);
	if (displayFilters.diffFilter[4]) m_customDiffLabel->setColor({ 255, 255, 255 });
	else m_customDiffLabel->setColor({ 125, 125, 125 });
	auto customDiffBtn = CCMenuItemExt::createSpriteExtra(m_customDiffLabel, [this](auto) {
		changeDiffFilter(DifficultyFilter::Custom, m_customDiffLabel);

		for (int i = 0; i < 4; i++) {
			if (GlobalList::Filters::getDisplayFilters().diffFilter[i]) {
				changeDiffFilter(static_cast<DifficultyFilter>(i), i == 0 ? m_top50Label : i == 1 ? m_top150Label : i == 2 ? m_top300Label : m_unboundedLabel);
			}
		}
	});
	diffFilterMenu->addChild(customDiffBtn);

	auto customDiffOptionsBtn = CCMenuItemExt::createSpriteExtra(optionsSpr, [this](auto) { RangePopup::create(FilterType::Difficulty)->show(); });
	customDiffOptionsBtn->updateLayout();
	diffFilterMenu->addChild(customDiffOptionsBtn);

	diffFilterMenu->updateLayout();


	auto lengthFilterBG = CCScale9Sprite::create("square02b_001.png");
	lengthFilterBG->setColor({ 123, 68, 41 });
	lengthFilterBG->setContentSize({ 365.0f, 35.0f });
	lengthFilterBG->setPosition({ 225.0f, 180.0f });
	m_mainLayer->addChild(lengthFilterBG);

	auto lengthFilterMenu = CCMenu::create();
	lengthFilterMenu->setLayout(RowLayout::create()
		->setGap(16.0f)
		->setAxisAlignment(AxisAlignment::Between)
		->setAutoScale(false));
	lengthFilterMenu->setContentSize({ 340.0f, 35.0f });
	lengthFilterMenu->setPosition({ 225.0f, 180.0f });
	lengthFilterMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(lengthFilterMenu);

	auto clockSpr = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	clockSpr->setContentSize({ 23.0f, 23.0f });
	lengthFilterMenu->addChild(clockSpr);

	m_shortLenLabel = CCLabelBMFont::create("Short", "bigFont.fnt");
	m_shortLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[0]) m_shortLenLabel->setColor({ 255, 255, 255 });
	else m_shortLenLabel->setColor({ 125, 125, 125 });
	auto shortLenBtn = CCMenuItemExt::createSpriteExtra(m_shortLenLabel, [this](auto) {
		changeLengthFilter(LengthFilter::Short, m_shortLenLabel);
	});
	lengthFilterMenu->addChild(shortLenBtn);

	m_mediumLenLabel = CCLabelBMFont::create("Medium", "bigFont.fnt");
	m_mediumLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[1]) m_mediumLenLabel->setColor({ 255, 255, 255 });
	else m_mediumLenLabel->setColor({ 125, 125, 125 });
	auto mediumLenBtn = CCMenuItemExt::createSpriteExtra(m_mediumLenLabel, [this](auto) {
		changeLengthFilter(LengthFilter::Medium, m_mediumLenLabel);
	});
	lengthFilterMenu->addChild(mediumLenBtn);

	m_longLenLabel = CCLabelBMFont::create("Long", "bigFont.fnt");
	m_longLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[2]) m_longLenLabel->setColor({ 255, 255, 255 });
	else m_longLenLabel->setColor({ 125, 125, 125 });
	auto longLenBtn = CCMenuItemExt::createSpriteExtra(m_longLenLabel, [this](auto) {
		changeLengthFilter(LengthFilter::Long, m_longLenLabel);
	});
	lengthFilterMenu->addChild(longLenBtn);

	m_xlLenLabel = CCLabelBMFont::create("XL", "bigFont.fnt");
	m_xlLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[3]) m_xlLenLabel->setColor({ 255, 255, 255 });
	else m_xlLenLabel->setColor({ 125, 125, 125 });
	auto xlLenBtn = CCMenuItemExt::createSpriteExtra(m_xlLenLabel, [this](auto) {
		changeLengthFilter(LengthFilter::XL, m_xlLenLabel);
	});
	lengthFilterMenu->addChild(xlLenBtn);

	m_customLenLabel = CCLabelBMFont::create("Custom", "bigFont.fnt");
	m_customLenLabel->setScale(0.42f);
	if (displayFilters.lengthFilter[4]) m_customLenLabel->setColor({ 255, 255, 255 });
	else m_customLenLabel->setColor({ 125, 125, 125 });
	auto customLenBtn = CCMenuItemExt::createSpriteExtra(m_customLenLabel, [this](auto) {
		changeLengthFilter(LengthFilter::Custom, m_customLenLabel);

		for (int i = 0; i < 4; i++) {
			if (GlobalList::Filters::getDisplayFilters().lengthFilter[i]) {
				changeLengthFilter(static_cast<LengthFilter>(i), i == 0 ? m_shortLenLabel : i == 1 ? m_mediumLenLabel : i == 2 ? m_longLenLabel : m_xlLenLabel);
			}
		}
	});
	lengthFilterMenu->addChild(customLenBtn);

	auto customLenOptionsBtn = CCMenuItemExt::createSpriteExtra(optionsSpr, [this](auto) { RangePopup::create(FilterType::Length)->show(); });
	customLenOptionsBtn->updateLayout();
	lengthFilterMenu->addChild(customLenOptionsBtn);

	lengthFilterMenu->updateLayout();


	auto otherFiltersBG = CCScale9Sprite::create("square02b_001.png");
	otherFiltersBG->setColor({ 123, 68, 41 });
	otherFiltersBG->setContentSize({ 365.0f, 100.0f });
	otherFiltersBG->setPosition({ 225.0f, 102.5f });
	m_mainLayer->addChild(otherFiltersBG);

	auto otherFiltersMenu = CCMenu::create();
	otherFiltersMenu->setContentSize({ 365.0f, 100.0f });
	otherFiltersMenu->setPosition({ 225.0f, 102.5f });
	otherFiltersMenu->ignoreAnchorPointForPosition(false);
	m_mainLayer->addChild(otherFiltersMenu);

	m_ratedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) {
			GlobalList::Filters::setRateFilter(true, false);
			m_unratedFilterToggler->toggle(false);
		}
		else GlobalList::Filters::setRateFilter(false, false);
	});
	m_ratedFilterToggler->toggle(displayFilters.rated);
	m_ratedFilterToggler->setScale(0.75f);
	m_ratedFilterToggler->setPosition({ 19.0f, 81.0f });
	otherFiltersMenu->addChild(m_ratedFilterToggler);

	auto ratedFilterLabel = CCLabelBMFont::create("Rated", "bigFont.fnt");
	ratedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	ratedFilterLabel->setScale(0.4f);
	ratedFilterLabel->setPosition({ 36.0f, 81.0f });
	otherFiltersMenu->addChild(ratedFilterLabel);

	m_unratedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) {
			GlobalList::Filters::setRateFilter(false, true);
			m_ratedFilterToggler->toggle(false);
		}
		else GlobalList::Filters::setRateFilter(false, false);
	});
	m_unratedFilterToggler->toggle(displayFilters.unrated);
	m_unratedFilterToggler->setScale(0.75f);
	m_unratedFilterToggler->setPosition({ 19.0f, 50.0f });
	otherFiltersMenu->addChild(m_unratedFilterToggler);

	auto unratedFilterLabel = CCLabelBMFont::create("Unrated", "bigFont.fnt");
	unratedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	unratedFilterLabel->setScale(0.4f);
	unratedFilterLabel->setPosition({ 36.0f, 50.0f });
	otherFiltersMenu->addChild(unratedFilterLabel);

	auto completedFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) GlobalList::Filters::setCompleted(true);
		else GlobalList::Filters::setCompleted(false);
	});
	completedFilterToggler->toggle(displayFilters.completed);
	completedFilterToggler->setScale(0.75f);
	completedFilterToggler->setPosition({ 143.5f, 81.0f });
	otherFiltersMenu->addChild(completedFilterToggler);

	auto completedFilterLabel = CCLabelBMFont::create("Completed", "bigFont.fnt");
	completedFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	completedFilterLabel->setScale(0.4f);
	completedFilterLabel->setPosition({ 160.5f, 81.0f });
	otherFiltersMenu->addChild(completedFilterLabel);

	auto creatorFilterToggler = CCMenuItemExt::createTogglerWithStandardSprites(1.0f, [this](CCMenuItemToggler* toggler) {
		if (!toggler->isOn()) GlobalList::Filters::setByHolder(true);
		else GlobalList::Filters::setByHolder(false);
	});
	creatorFilterToggler->toggle(displayFilters.byHolder);
	creatorFilterToggler->setScale(0.75f);
	creatorFilterToggler->setPosition({ 143.5f, 50.0f });
	otherFiltersMenu->addChild(creatorFilterToggler);

	auto creatorFilterLabel = CCLabelBMFont::create("Creator", "bigFont.fnt");
	creatorFilterLabel->setAnchorPoint({ 0.0f, 0.5f });
	creatorFilterLabel->setScale(0.4f);
	creatorFilterLabel->setPosition({ 160.5f, 50.0f });
	otherFiltersMenu->addChild(creatorFilterLabel);

	auto usernameInput = TextInput::create(106.0f / 0.9f, "Username");
	usernameInput->setAnchorPoint({ 0.0f, 0.5f });
	usernameInput->setScale(0.9f);
	usernameInput->setPosition({ 254.0f, 81.0f });
	usernameInput->setMaxCharCount(32);
	usernameInput->setString(Mod::get()->getSavedValue<std::string>("username", ""));
	usernameInput->setCallback([this](const std::string& text) {
		Mod::get()->setSavedValue<std::string>("username", text);
	});
	otherFiltersMenu->addChild(usernameInput);

	auto holderInput = TextInput::create(106.0f / 0.9f, "Holder");
	holderInput->setAnchorPoint({ 0.0f, 0.5f });
	holderInput->setScale(0.9f);
	holderInput->setPosition({ 254.0f, 50.0f });
	holderInput->setMaxCharCount(32);
	holderInput->setString(Mod::get()->getSavedValue<std::string>("holder", ""));
	holderInput->setCallback([this](const std::string& text) {
		Mod::get()->setSavedValue<std::string>("holder", text);
	});
	otherFiltersMenu->addChild(holderInput);
	
	auto applyBtnSpr = ButtonSprite::create("Apply");
	auto applyBtn = CCMenuItemExt::createSpriteExtra(applyBtnSpr, [this, usernameInput, holderInput](auto) {
		auto now = std::time(nullptr);

		auto lastUpdate = GlobalList::Cache::getLastUpdateTime();
		auto difference = now - lastUpdate;

		auto& levelFilters = GlobalList::Filters::getDisplayFilters();
		if (!levelFilters.isDataRequired() || (GlobalList::Cache::levelDataSize() == 0 ? false : difference < 1800)) {
			GlobalList::Filters::applyFilters();
			PopulateListEvent().send();
			
			onClose(this);
			return;
		}

		GlobalList::Filters::setUsername(usernameInput->getString());
		GlobalList::Filters::setHolderName(holderInput->getString());
		
		if (!Mod::get()->getSavedValue<bool>("showWarning", true)) {
			LoadingPopup::create()->show();
		}
		else {
			createQuickPopup("Warning", filterWarning, "Oh, no", "I confirm", [](auto, bool confirmBtn) {
				if (confirmBtn) {
					Mod::get()->setSavedValue<bool>("showWarning", false);
					LoadingPopup::create()->show();
				}
			});
		}

	});
	applyBtn->setPosition({ 225.0f, 28.0f });
	m_buttonMenu->addChild(applyBtn);

	auto infoBtn = InfoAlertButton::create("How to use filters", filterInfo, 1.0f);
	infoBtn->setPosition({24.0f, 24.0f});
	m_buttonMenu->addChild(infoBtn);

	m_closeListener = MyCloseEvent(PopupType::FilterPopup).listen(
		[this]() { Popup::onClose(this); }
	);

	return true;
}

void FilterPopup::changeLengthFilter(LengthFilter type, CCLabelBMFont* label) {
	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	bool lengthFilter = displayFilters.lengthFilter[(int)type];
	if (!lengthFilter) {
		label->setColor({ 255, 255, 255 });
		GlobalList::Filters::setLengthFilter(type, true);
	}
	else {
		label->setColor({ 125, 125, 125 });
		GlobalList::Filters::setLengthFilter(type, false);
	}
}

void FilterPopup::changeDiffFilter(DifficultyFilter type, CCLabelBMFont* label) {
	auto& displayFilters = GlobalList::Filters::getDisplayFilters();
	bool diffFilter = displayFilters.diffFilter[(int)type];
	if (!diffFilter) {
		label->setColor({ 255, 255, 255 });
		GlobalList::Filters::setDifficultyFilter(type, true);
	}
	else {
		label->setColor({ 125, 125, 125 });
		GlobalList::Filters::setDifficultyFilter(type, false);
	}
}