document.addEventListener("DOMContentLoaded", () => {
    const codeTextarea = document.querySelector(".code");
    const inputField = document.getElementById("inputField");
    const runButton = document.getElementById("runButton");
    const nextButton = document.getElementById("nextButton");
    const resetButton = document.getElementById("resetButton");
    const pcView = document.querySelector(".PC_view");
    let isRegMode = true;
    let resetd = false;

    let pc = 0; // 초기 Program Counter 값
    // nextButton.disabled = true;
    nextButton.disabled = false;

inputField.value = `    .data 0x1000
    .text 0x2000
    .globl main
main:
# write code here


# program end
li $v0, 10
syscall

`;

    

    // 로딩 창 보이기
    function showLoading() {
        document.getElementById("loadingPopup").style.display = "flex";
    }

    // 로딩 창 숨기기
    function hideLoading() {
        document.getElementById("loadingPopup").style.display = "none";
    }

    // Function to display the fetched data
    function displayData(data) {

        // Display MEMWB data (reverse order)
        const memwb = data.MEMWB;
        document.querySelector('#MEMWB .data-section').innerHTML = `
            <span><strong>MemtoReg:</strong> ${memwb.signal_wb.MemtoReg}</span> 
            <span><strong>RegWrite:</strong> ${memwb.signal_wb.RegWrite}</span> 
            <span><strong>next_pc:</strong> ${memwb.next_pc}</span> 
            <span><strong>wb_data:</strong> ${memwb.wb_data}</span> 
            <span><strong>rd:</strong> ${memwb.rd}</span> 
        `;

        // Display EXMEM data (reverse order)
        const exmem = data.EXMEM;
        document.querySelector('#EXMEM .data-section').innerHTML = `
            <span><strong>MemtoReg:</strong> ${exmem.signal_wb.MemtoReg}</span> 
            <span><strong>RegWrite:</strong> ${exmem.signal_wb.RegWrite}</span> 
            <span><strong>MemRead:</strong> ${exmem.signal_mem.MemRead}</span> 
            <span><strong>JumpReg:</strong> ${exmem.signal_mem.JumpReg}</span> 
            <span><strong>MemWrite:</strong> ${exmem.signal_mem.MemWrite}</span> 
            <span><strong>Branch:</strong> ${exmem.signal_mem.Branch}</span> 
            <span><strong>Jump:</strong> ${exmem.signal_mem.Jump}</span> 
            <span><strong>next_pc:</strong> ${exmem.next_pc}</span> 
            <span><strong>zero:</strong> ${exmem.zero}</span> 
            <span><strong>ALUOut:</strong> ${exmem.ALUOut}</span> 
            <span><strong>b:</strong> ${exmem.b}</span> 
            <span><strong>rd:</strong> ${exmem.rd}</span> 
        `;

        // Display IDEX data (reverse order)
        const idex = data.IDEX;
        document.querySelector('#IDEX .data-section').innerHTML = `
            <span><strong>IDEXWrite:</strong> ${idex.IDEXWrite}</span> 
            <span><strong>flush:</strong> ${idex.flush}</span> 
            <span><strong>MemRead:</strong> ${idex.signal_mem.MemRead}</span> 
            <span><strong>JumpReg:</strong> ${idex.signal_mem.JumpReg}</span> 
            <span><strong>MemWrite:</strong> ${idex.signal_mem.MemWrite}</span> 
            <span><strong>Branch:</strong> ${idex.signal_mem.Branch}</span> 
            <span><strong>Jump:</strong> ${idex.signal_mem.Jump}</span> 
            <span><strong>RegDst:</strong> ${idex.signal_ex.RegDst}</span> 
            <span><strong>ALUSrc:</strong> ${idex.signal_ex.ALUSrc}</span> 
            <span><strong>ALUOp:</strong> ${idex.signal_ex.ALUOp}</span> 
            <span><strong>next_pc:</strong> ${idex.next_pc}</span> 
            <span><strong>a:</strong> ${idex.a}</span> 
            <span><strong>b:</strong> ${idex.b}</span> 
            <span><strong>constant:</strong> ${idex.constant}</span> 
            <span><strong>rs:</strong> ${idex.rs}</span>
            <span><strong>rt:</strong> ${idex.rt}</span> 
            <span><strong>rd:</strong> ${idex.rd}</span> 
        `;

        // Display IFID data (reverse order)
        const ifid = data.IFID;
        document.querySelector('#IFID .data-section').innerHTML = `
            <span><strong>IFIDWrite:</strong> ${ifid.IFIDWrite}</span> 
            <span><strong>flush:</strong> ${ifid.flush}</span> 
            <span><strong>next_pc:</strong> ${ifid.next_pc}</span> 
            <span><strong>instruction:</strong> ${ifid.instruction}</span> 
        `;

    }


    async function fetchData() {
        nextButton.disabled = true;
        try{
            showLoading();
            const response = await fetch('/next');
            hideLoading();
            if (!response.ok){
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            const data = await response.json();
            pcView.textContent = "PC : " + data.pc;
            displayData(data);
        } catch(error){
            console.error("Error fetching data:", error);
        } finally{
            nextButton.disabled = false;
        }
    }
    
    async function uploadFormData(formData, type) {
        resetd = false;
        try {
            showLoading();
            const response = await fetch('/upload', {
                method: 'POST',
                body: formData
            });
            hideLoading();
            if (!response.ok){
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            const data = await response.json();
            inputField.value = data.file_content;
                        
        } catch (error) {
            console.error("Error: ", error);
        }
    }

    async function uploadData(formData, type) {
        resetd = false;
        try {
            showLoading();
            const response = await fetch('/upload', {
                method: 'POST',
                headers: {
                    "Content-Type": type,
                },
                body: formData
            });
            hideLoading();
            if (!response.ok){
                throw new Error(`HTTP error! Status: ${response.status}`);
            }
            const data = await response.json();
            inputField.value = data.file_content;
                        
        } catch (error) {
            console.error("Error: ", error);
        }
    }

    async function resetData(){
        resetButton.disabled = true;
        resetd = true;
        try{
            showLoading();
            const response = await fetch('/reset')
            hideLoading();
            if(!response.ok){
                throw new Error(`Reset failed: ${response.status}`);
            }
            const data = await response.json();
            pcView.textContent = "PC : " + data.pc;
            displayData(data);

        } catch (error){
            console.error("Error: ",error);
        } finally{
            nextButton.disabled = false;
            resetButton.disabled = false;
        }
            
    }

    function run(){
        
    }


    // 드래그한 파일이 텍스트 영역에 놓일 때 실행되는 함수
    inputField.addEventListener("dragover",(event)=>{
        event.preventDefault();
        inputField.style.backgroundColor = '#f0f0f0'; // 드래그 오버 시 배경 색 변경
    });
    // 드래그한 파일이 텍스트 영역에 놓아졌을 때 실행되는 함수
    inputField.addEventListener('dragleave', () => {
        inputField.style.backgroundColor = ''; // 드래그가 끝난 후 배경 색 원래대로
    });
    inputField.addEventListener('drop', async (event) => {
        event.preventDefault();  // 기본 동작 방지 (파일 열기 등)

        const file = event.dataTransfer.files[0];  // 첫 번째 파일을 가져옴
        if (!file) {
            alert('Please select a file to upload.');
            return;
        }

        const formData = new FormData();
        formData.append("asm", file); 

        await uploadFormData(formData);
        
        inputField.style.backgroundColor = '';
        
    });


    inputField.addEventListener("input", () => {
        nextButton.disabled = true;
        runButton.setAttribute("data-state", "play");
        resetd = false;
    });

    // runButton.addEventListener("click", async ()=>{
    //     if(runButton.getAttribute("data-state") === "play"){
    //         await uploadData(inputField.value, "text/plain");
    //         if(!resetd) await resetData();
    //         fetch('/run')
    //         .then(data => data.json())
    //         .then(data =>{
    //             pcView.textContent = "PC : " + data.pc;
    //             displayData(data);
    //         });
    //         runButton.setAttribute("data-state", "stop");
    //         nextButton.disabled = true;
    //     } else{
    //         await fetch('/stop');
    //         runButton.setAttribute("data-state", "play");
    //         nextButton.disabled = false;
    //     }
        
    // });
    nextButton.addEventListener("click", async ()=>{
        if (!resetd){
            await resetData();
        }
        await fetchData();
    });
    resetButton.addEventListener("click", async ()=>{
        nextButton.disabled = true;
        runButton.setAttribute("data-state", "play");
        await resetData();
        runButton.setAttribute("data-state", "stop");
    });

    const dataInput =  document.getElementById('dataInput');
    const modelLabel = document.getElementById('modeLabel');
    dataInput.addEventListener("input", () => {
        if (dataInput.value[0] !== '$' && modelLabel.innerText === "REG"){
            dataInput.value = '$';
        }
    });

    modelLabel.addEventListener('click', () =>{
        if (isRegMode) {
            modelLabel.innerText = "MEM";  // Change mode label to MEM
            modelLabel.style.backgroundColor = "yellow";
            modelLabel.style.color = "gray";
            dataInput.value = '';
            
        } else {
            modelLabel.innerText = "REG";  // Change mode label to REG
            modelLabel.style.backgroundColor = "gray";
            modelLabel.style.color = "yellow";
            dataInput.value = '$';
        }
        
        isRegMode = !isRegMode;  // Toggle between REG and MEM mode
    });

    document.getElementById('read').addEventListener('click', async () => {
        let InputValue = document.getElementById('dataInput').value;
        if (InputValue) {
            if (isRegMode) {
                InputValue = InputValue.substring(1);
                showLoading();
                const response = await fetch(`/read_reg?data=${encodeURIComponent(InputValue)}`);
                hideLoading();
                const reg_data = await response.json();
                document.getElementById('data').innerText = `REG[$${InputValue}] : ${reg_data.content}`;
                InputValue.value = '$';
            } else {
                showLoading();
                const response = await fetch(`/read_mem?data=${encodeURIComponent(InputValue)}`);
                hideLoading();
                const mem_data = await response.json();
                document.getElementById('data').innerText = `MEM[${InputValue}] : ${mem_data.content}`;
                InputValue.value = '';

            }
              // Clear input after submission
        } else {
            alert('Please enter data');
        }
    });


    
// 팝업을 여는 버튼 클릭
document.getElementById('searchButton').addEventListener('click', function() {
    document.getElementById('popupForm').style.display = 'block';
});

// 팝업을 닫는 버튼 클릭
document.getElementById('closePopup').addEventListener('click', function() {
    document.getElementById('popupForm').style.display = 'none';
});



});
