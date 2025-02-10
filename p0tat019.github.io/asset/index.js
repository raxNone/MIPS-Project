document.addEventListener("DOMContentLoaded", () => {
    const codeTextarea = document.querySelector(".code");
    const inputField = document.getElementById("inputField");
    const runButton = document.getElementById("runButton");
    const nextButton = document.getElementById("nextButton");
    const resetButton = document.getElementById("resetButton");
    const pcView = document.querySelector(".PC_view");
    let isRegMode = true;

    let pc = 0; // 초기 Program Counter 값
    nextButton.disabled = true;

        // Function to display the fetched data
    function displayData(data) {

        // Display MEMWB data
        const memwb = data.MEMWB;
        document.getElementById('MEMWB').innerHTML = `
            <strong>rd:</strong> ${memwb.rd} <br>
            <strong>wb_data:</strong> ${memwb.wb_data} <br>
            <strong>next_pc:</strong> ${memwb.next_pc} <br>
            <strong>RegWrite:</strong> ${memwb.signal_wb.RegWrite} <br>
            <strong>MemtoReg:</strong> ${memwb.signal_wb.MemtoReg} <br>
        `;

        // Display EXMEM data
        const exmem = data.EXMEM;
        document.getElementById('EXMEM').innerHTML = `
            <strong>rd:</strong> ${exmem.rd} <br>
            <strong>b:</strong> ${exmem.b} <br>
            <strong>ALUOut:</strong> ${exmem.ALUOut} <br>
            <strong>zero:</strong> ${exmem.zero} <br>
            <strong>next_pc:</strong> ${exmem.next_pc} <br>
            <strong>Jump:</strong> ${exmem.signal_mem.Jump} <br>
            <strong>Branch:</strong> ${exmem.signal_mem.Branch} <br>
            <strong>MemWrite:</strong> ${exmem.signal_mem.MemWrite} <br>
            <strong>JumpReg:</strong> ${exmem.signal_mem.JumpReg} <br>
            <strong>MemRead:</strong> ${exmem.signal_mem.MemRead} <br>
            <strong>RegWrite:</strong> ${exmem.signal_wb.RegWrite} <br>
            <strong>MemtoReg:</strong> ${exmem.signal_wb.MemtoReg} <br>
        `;

        // Display IDEX data
        const idex = data.IDEX;
        document.getElementById('IDEX').innerHTML = `
            <strong>rd:</strong> ${idex.rd} <br>
            <strong>rs:</strong> ${idex.rs} <br>
            <strong>b:</strong> ${idex.b} <br>
            <strong>a:</strong> ${idex.a} <br>
            <strong>ALUOp:</strong> ${idex.signal_ex.ALUOp} <br>
            <strong>ALUSrc:</strong> ${idex.signal_ex.ALUSrc} <br>
            <strong>RegDst:</strong> ${idex.signal_ex.RegDst} <br>
            <strong>Jump:</strong> ${idex.signal_mem.Jump} <br>
            <strong>Branch:</strong> ${idex.signal_mem.Branch} <br>
            <strong>MemWrite:</strong> ${idex.signal_mem.MemWrite} <br>
            <strong>JumpReg:</strong> ${idex.signal_mem.JumpReg} <br>
            <strong>MemRead:</strong> ${idex.signal_mem.MemRead} <br>
            <strong>rt:</strong> ${idex.rt} <br>
            <strong>next_pc:</strong> ${idex.next_pc} <br>
            <strong>flush:</strong> ${idex.flush} <br>
            <strong>IDEXWrite:</strong> ${idex.IDEXWrite} <br>
            <strong>constant:</strong> ${idex.constant} <br>
        `;

        // Display IFID data
        const ifid = data.IFID;
        document.getElementById('IFID').innerHTML = `
            <strong>instruction:</strong> ${ifid.instruction} <br>
            <strong>next_pc:</strong> ${ifid.next_pc} <br>
            <strong>flush:</strong> ${ifid.flush} <br>
            <strong>IFIDWrite:</strong> ${ifid.IFIDWrite} <br>
        `;
    }


    async function fetchData() {
        nextButton.disabled = true;
        try{
            const response = await fetch('/next');
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
        try {
            const response = await fetch('/upload', {
                method: 'POST',
                body: formData
            });
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
        try {
            const response = await fetch('/upload', {
                method: 'POST',
                headers: {
                    "Content-Type": type,
                },
                body: formData
            });
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
        try{
            const response = await fetch('/reset')
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
    });

    runButton.addEventListener("click", async ()=>{
        if(runButton.getAttribute("data-state") === "play"){
            await uploadData(inputField.value, "text/plain");
            await resetData();
            runButton.setAttribute("data-state", "stop");
            nextButton.disabled = false;
        } else{
            runButton.setAttribute("data-state", "play");
            nextButton.disabled = true;
        }
        
    });
    nextButton.addEventListener("click", async ()=>{
        await fetchData();
    });
    resetButton.addEventListener("click", async ()=>{
        nextButton.disabled = true;
        runButton.setAttribute("data-state", "play");
        await resetData();
        runButton.setAttribute("data-state", "stop");
    });


    document.getElementById('modeLabel').addEventListener('click', () =>{
        if (isRegMode) {
            document.getElementById('modeLabel').innerText = "MEM";  // Change mode label to MEM
            document.getElementById('modeLabel').style.backgroundColor = "yellow";
            document.getElementById('dataInput').placeholder = "Enter Mem Addr";
            
        } else {
            document.getElementById('modeLabel').innerText = "REG";  // Change mode label to REG
            document.getElementById('modeLabel').style.backgroundColor = "gray";
            document.getElementById('dataInput').placeholder = "Enter Reg Num"
            
        }
        document.getElementById('dataInput').value = '';  // Clear input after submission
        isRegMode = !isRegMode;  // Toggle between REG and MEM mode
    });

    document.getElementById('read').addEventListener('click', async () => {
        var dataInput = document.getElementById('dataInput').value;
        if (dataInput) {
            if (isRegMode) {
                const response = await fetch(`/read_reg?data=${encodeURIComponent(dataInput)}`);
                const reg_data = await response.json();
                document.getElementById('data').innerText = `REG[${dataInput}] : ${reg_data.content}`;
                
            } else {
                const response = await fetch(`/read_mem?data=${encodeURIComponent(dataInput)}`);
                const mem_data = await response.json();
                document.getElementById('data').innerText = `MEM[${dataInput}] : ${mem_data.content}`;

            }
            document.getElementById('dataInput').value = '';  // Clear input after submission
        } else {
            alert('Please enter data');
        }
    });






});
